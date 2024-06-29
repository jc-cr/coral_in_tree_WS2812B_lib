.syntax unified
.thumb
.thumb_func

.equ GPIO2_BASE,    0x40130000
.equ GPIO_DR,       0x00
.equ GPIO_GDIR,     0x04
.equ GPIO_DR_SET,   0x84
.equ GPIO_DR_CLEAR, 0x88
.equ LED_PIN,       31  // Assuming it's pin 31 (GPIO_MUX2_IO31)

.global _ZN10coralmicro7SendBitEb
.type _ZN10coralmicro7SendBitEb, %function

// Send a single bit to the WS2812B LED
// Timing specs:
// T0H: 400 ns  : -150 = 250 ns
// T1H: 800 ns  : -150 = 650 ns
// T0L: 850 ns  : -150 = 700 ns
// T1L: 450 ns  : -150 = 300 ns
// Total: 1.25 μs
// Delay:50,000 μs (50 ms) @ 800MHz = 40,000,000 cycles but according to https://wp.josh.com/2014/05/13/ws2812-neopixels-are-not-so-finicky-once-you-get-to-know-them/
//  we can do at 6,000 μs (6 ms) @ 800MHz = 4,800,000 cycles
//  After trying, can confirm that 6ms is enough to reset the LED
// In our code we want to aim for the lower range of the timings to be safe
_ZN10coralmicro7SendBitEb:
    // r0 contains the bit value (0 or 1)
    push {r4-r6, lr}
    
    // Load GPIO base address
    ldr r4, =GPIO2_BASE
    
    // Prepare pin mask
    movs r5, #1
    lsls r5, r5, #LED_PIN
    
    // Set GPIO high
    str r5, [r4, #GPIO_DR_SET]
    
    // Delay for T0H or T1H
    cmp r0, #0
    beq .zero_bit
    // T1H delay (650ns @ 800MHz = 520 cycles)
    movs r6, #173  // 520/3 (rounded down)
1:  subs r6, r6, #1
    bne 1b
    b .end_high
.zero_bit:
    // T0H delay (250ns @ 800MHz = 200 cycles)
    movs r6, #67  // 200/3 (rounded down)
1:  subs r6, r6, #1
    bne 1b
.end_high:

    // Set GPIO low
    str r5, [r4, #GPIO_DR_CLEAR]
    
    // Delay for T0L or T1L
    cmp r0, #0
    beq .zero_bit_low
    // T1L delay (300ns @ 800MHz = 240 cycles)
    movs r6, #80  // 240/3 (rounded down)
1:  subs r6, r6, #1
    bne 1b
    b .end
.zero_bit_low:
    // T0L delay (700ns @ 800MHz = 560 cycles)
    movs r6, #187  // 560/3 (rounded down)
1:  subs r6, r6, #1
    bne 1b
.end:
    pop {r4-r6, pc}

.global _ZN10coralmicro15InitializeGpioEv
.type _ZN10coralmicro15InitializeGpioEv, %function

_ZN10coralmicro15InitializeGpioEv:
    push {r4-r5, lr}
    
    // Load GPIO base address
    ldr r4, =GPIO2_BASE
    
    // Prepare pin mask
    movs r5, #1
    lsls r5, r5, #LED_PIN
    
    // Set pin as output
    ldr r0, [r4, #GPIO_GDIR]
    orrs r0, r0, r5
    str r0, [r4, #GPIO_GDIR]
    
    pop {r4-r5, pc}

.global _ZN10coralmicro10ResetDelayEv
.type _ZN10coralmicro10ResetDelayEv, %function

_ZN10coralmicro10ResetDelayEv:
    push {r4-r6, lr}
    
    // Load GPIO base address
    ldr r4, =GPIO2_BASE
    
    // Prepare pin mask
    movs r5, #1
    lsls r5, r5, #LED_PIN
    
    // Set GPIO low
    str r5, [r4, #GPIO_DR_CLEAR]
    
    // Delay for 50,000 μs (50 ms) @ 800MHz = 40,000,000 cycles
    // Delay for 6, 00 μs (6 ms) @ 800MHz = 4,800,000 cycles
    ldr r6, =1600000 // 4,800,000/3 (rounded down)
1:  subs r6, r6, #1
    bne 1b
    
    pop {r4-r6, pc}