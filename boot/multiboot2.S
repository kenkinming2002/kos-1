#define MULTIBOOT2_MAGIC         0xE85250D6
#define MULTIBOOT2_ARCHITECTURE  0x0
#define MULTIBOOT2_HEADER_LENGTH multiboot2_header_end - multiboot2_header_begin
#define MULTIBOOT2_CHECKSUM      -(MULTIBOOT2_MAGIC+MULTIBOOT2_HEADER_LENGTH+MULTIBOOT2_ARCHITECTURE)

.section .multiboot2
  .align 8
  multiboot2_header_begin:
    .long MULTIBOOT2_MAGIC
    .long MULTIBOOT2_ARCHITECTURE
    .long MULTIBOOT2_HEADER_LENGTH
    .long MULTIBOOT2_CHECKSUM

    .short 1
    .short 0
    .long  12
    .long  6
    .long  0

    .short 0
    .short 0
    .long  8
  multiboot2_header_end:
