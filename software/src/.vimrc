let g:syntastic_c_compiler = 'avr-gcc'
let g:syntastic_c_check_header = 1
let g:syntastic_c_compiler_options = '-mmcu=atxmega128a4u -Os -DF_CPU=32000000UL -DARCH=ARCH_XMEGA -DF_USB=48000000UL'
