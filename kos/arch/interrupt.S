.global interrupt_load
.extern idt_desc
.extern isr

isr_common:
  push %rax
  push %rcx
  push %rdx

  push %rdi
  push %rsi

  push %r8
  push %r9
  push %r10
  push %r11

  mov 0x48(%rsp), %rdi /* Index */
  mov 0x50(%rsp), %rsi /* Error code */
  call isr

  pop %r11
  pop %r10
  pop %r9
  pop %r8

  pop %rsi
  pop %rdi

  pop %rdx
  pop %rcx
  pop %rax

  add $0x10, %rsp
  iretq

.macro isr_no_error_code n
.global isr\n
isr\n:
  push $0x0
  push $\n
  jmp isr_common
.endm

.macro isr_error_code n
.global isr\n
isr\n:
  push $\n
  jmp isr_common
.endm

isr_no_error_code 0
isr_no_error_code 1
isr_no_error_code 2
isr_no_error_code 3
isr_no_error_code 4
isr_no_error_code 5
isr_no_error_code 6
isr_no_error_code 7
isr_error_code    8  /* double fault */
isr_no_error_code 9
isr_error_code    10 /* invalid TSS */
isr_error_code    11 /* segment not present */
isr_error_code    12 /* stack segment fault */
isr_error_code    13 /* general protection fault */
isr_error_code    14 /* page fault */
isr_no_error_code 15
isr_no_error_code 16
isr_error_code 17    /* alignment check */
isr_no_error_code 18
isr_no_error_code 19
isr_no_error_code 20
isr_error_code    21 /* control protection exception */
isr_no_error_code 22
isr_no_error_code 23
isr_no_error_code 24
isr_no_error_code 25
isr_no_error_code 26
isr_no_error_code 27
isr_no_error_code 28
isr_error_code    29 /* vmm communication exception */
isr_error_code    30 /* security exception */
isr_no_error_code 31
isr_no_error_code 32
isr_no_error_code 33
isr_no_error_code 34
isr_no_error_code 35
isr_no_error_code 36
isr_no_error_code 37
isr_no_error_code 38
isr_no_error_code 39
isr_no_error_code 40
isr_no_error_code 41
isr_no_error_code 42
isr_no_error_code 43
isr_no_error_code 44
isr_no_error_code 45
isr_no_error_code 46
isr_no_error_code 47
isr_no_error_code 48
isr_no_error_code 49
isr_no_error_code 50
isr_no_error_code 51
isr_no_error_code 52
isr_no_error_code 53
isr_no_error_code 54
isr_no_error_code 55
isr_no_error_code 56
isr_no_error_code 57
isr_no_error_code 58
isr_no_error_code 59
isr_no_error_code 60
isr_no_error_code 61
isr_no_error_code 62
isr_no_error_code 63
isr_no_error_code 64
isr_no_error_code 65
isr_no_error_code 66
isr_no_error_code 67
isr_no_error_code 68
isr_no_error_code 69
isr_no_error_code 70
isr_no_error_code 71
isr_no_error_code 72
isr_no_error_code 73
isr_no_error_code 74
isr_no_error_code 75
isr_no_error_code 76
isr_no_error_code 77
isr_no_error_code 78
isr_no_error_code 79
isr_no_error_code 80
isr_no_error_code 81
isr_no_error_code 82
isr_no_error_code 83
isr_no_error_code 84
isr_no_error_code 85
isr_no_error_code 86
isr_no_error_code 87
isr_no_error_code 88
isr_no_error_code 89
isr_no_error_code 90
isr_no_error_code 91
isr_no_error_code 92
isr_no_error_code 93
isr_no_error_code 94
isr_no_error_code 95
isr_no_error_code 96
isr_no_error_code 97
isr_no_error_code 98
isr_no_error_code 99
isr_no_error_code 100
isr_no_error_code 101
isr_no_error_code 102
isr_no_error_code 103
isr_no_error_code 104
isr_no_error_code 105
isr_no_error_code 106
isr_no_error_code 107
isr_no_error_code 108
isr_no_error_code 109
isr_no_error_code 110
isr_no_error_code 111
isr_no_error_code 112
isr_no_error_code 113
isr_no_error_code 114
isr_no_error_code 115
isr_no_error_code 116
isr_no_error_code 117
isr_no_error_code 118
isr_no_error_code 119
isr_no_error_code 120
isr_no_error_code 121
isr_no_error_code 122
isr_no_error_code 123
isr_no_error_code 124
isr_no_error_code 125
isr_no_error_code 126
isr_no_error_code 127
isr_no_error_code 128
isr_no_error_code 129
isr_no_error_code 130
isr_no_error_code 131
isr_no_error_code 132
isr_no_error_code 133
isr_no_error_code 134
isr_no_error_code 135
isr_no_error_code 136
isr_no_error_code 137
isr_no_error_code 138
isr_no_error_code 139
isr_no_error_code 140
isr_no_error_code 141
isr_no_error_code 142
isr_no_error_code 143
isr_no_error_code 144
isr_no_error_code 145
isr_no_error_code 146
isr_no_error_code 147
isr_no_error_code 148
isr_no_error_code 149
isr_no_error_code 150
isr_no_error_code 151
isr_no_error_code 152
isr_no_error_code 153
isr_no_error_code 154
isr_no_error_code 155
isr_no_error_code 156
isr_no_error_code 157
isr_no_error_code 158
isr_no_error_code 159
isr_no_error_code 160
isr_no_error_code 161
isr_no_error_code 162
isr_no_error_code 163
isr_no_error_code 164
isr_no_error_code 165
isr_no_error_code 166
isr_no_error_code 167
isr_no_error_code 168
isr_no_error_code 169
isr_no_error_code 170
isr_no_error_code 171
isr_no_error_code 172
isr_no_error_code 173
isr_no_error_code 174
isr_no_error_code 175
isr_no_error_code 176
isr_no_error_code 177
isr_no_error_code 178
isr_no_error_code 179
isr_no_error_code 180
isr_no_error_code 181
isr_no_error_code 182
isr_no_error_code 183
isr_no_error_code 184
isr_no_error_code 185
isr_no_error_code 186
isr_no_error_code 187
isr_no_error_code 188
isr_no_error_code 189
isr_no_error_code 190
isr_no_error_code 191
isr_no_error_code 192
isr_no_error_code 193
isr_no_error_code 194
isr_no_error_code 195
isr_no_error_code 196
isr_no_error_code 197
isr_no_error_code 198
isr_no_error_code 199
isr_no_error_code 200
isr_no_error_code 201
isr_no_error_code 202
isr_no_error_code 203
isr_no_error_code 204
isr_no_error_code 205
isr_no_error_code 206
isr_no_error_code 207
isr_no_error_code 208
isr_no_error_code 209
isr_no_error_code 210
isr_no_error_code 211
isr_no_error_code 212
isr_no_error_code 213
isr_no_error_code 214
isr_no_error_code 215
isr_no_error_code 216
isr_no_error_code 217
isr_no_error_code 218
isr_no_error_code 219
isr_no_error_code 220
isr_no_error_code 221
isr_no_error_code 222
isr_no_error_code 223
isr_no_error_code 224
isr_no_error_code 225
isr_no_error_code 226
isr_no_error_code 227
isr_no_error_code 228
isr_no_error_code 229
isr_no_error_code 230
isr_no_error_code 231
isr_no_error_code 232
isr_no_error_code 233
isr_no_error_code 234
isr_no_error_code 235
isr_no_error_code 236
isr_no_error_code 237
isr_no_error_code 238
isr_no_error_code 239
isr_no_error_code 240
isr_no_error_code 241
isr_no_error_code 242
isr_no_error_code 243
isr_no_error_code 244
isr_no_error_code 245
isr_no_error_code 246
isr_no_error_code 247
isr_no_error_code 248
isr_no_error_code 249
isr_no_error_code 250
isr_no_error_code 251
isr_no_error_code 252
isr_no_error_code 253
isr_no_error_code 254
isr_no_error_code 255

.section .text
interrupt_load:
  lidt idt_desc(%rip)
  ret
