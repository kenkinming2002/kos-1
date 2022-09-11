#include "print.h"

#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>

#include "boot/debug.h"

static void debug_print_uintmax(uintmax_t val, unsigned base, const char *mappings)
{
  unsigned i = 0;
  char buf[32];

  for(uintmax_t tmp = val; tmp != 0; tmp /= base, ++i)
  {
    unsigned digit = tmp % base;
    buf[i] = mappings[digit];
  }
  for(; i != 0; --i)
    debug_put(buf[i-1]);
}

static void debug_print_intmax(intmax_t val, unsigned base, const char *mappings)
{
  if(val < 0)
  {
    debug_put('-');
    val = -val;
  }
  debug_print_uintmax(val, base, mappings);
}

static void debug_print_string(const char *str)
{
  for(;*str; ++str)
    debug_put(*str);
}

enum LengthModifier
{
  LENGTH_CHAR,
  LENGTH_SHORT,
  LENGTH_INT,
  LENGTH_LONG,
  LENGTH_LONG_LONG,
};

enum ConversionSpecifier
{
  CONVERSION_DEC_SIGNED,
  CONVERSION_DEC_UNSIGNED,
  CONVERSION_HEX_LOWERCASE,
  CONVERSION_HEX_UPPERCASE,
};

static bool consume(const char **p, char c)
{
  if(**p == c)
  {
    ++(*p);
    return true;
  }
  else
    return false;
}

void debug_printf(const char *format, ...)
{
  va_list args;
  va_start(args, format);

  const char *p = format;
  while(*p)
  {
    if(consume(&p, '%')) // Try to parse a format specifier
    {
      if(consume(&p, 's'))
      {
        // String
        debug_print_string(va_arg(args, const char *));
      }
      else
      {
        // Integer

        // Parse length modifier
        enum LengthModifier length_modifier;
        if(consume(&p, 'h'))
        {
          if(consume(&p, 'h'))
            length_modifier = LENGTH_CHAR;
          else
            length_modifier = LENGTH_SHORT;
        }
        else if(consume(&p, 'l'))
        {
          if(consume(&p, 'l'))
            length_modifier = LENGTH_LONG_LONG;
          else
            length_modifier = LENGTH_LONG;
        }
        else
          length_modifier = LENGTH_INT;

        // Parse conversion specifier
        enum ConversionSpecifier conversion_specifier;
        if(consume(&p, 'd') || consume(&p, 'i'))
          conversion_specifier = CONVERSION_DEC_SIGNED;
        else if(consume(&p, 'u'))
          conversion_specifier = CONVERSION_DEC_UNSIGNED;
        else if(consume(&p, 'x'))
          conversion_specifier = CONVERSION_HEX_LOWERCASE;
        else if(consume(&p, 'X'))
          conversion_specifier = CONVERSION_HEX_UPPERCASE;
        else
          continue; // WTH
                    //
        bool is_signed;
        unsigned base;
        const char *mappings;
        switch(conversion_specifier)
        {
        case CONVERSION_DEC_SIGNED:
          is_signed = true;
          base      = 10;
          mappings  = "0123456789";
          break;
        case CONVERSION_DEC_UNSIGNED:
          is_signed = false;
          base      = 10;
          mappings  = "0123456789";
          break;
        case CONVERSION_HEX_LOWERCASE:
          is_signed = false;
          base      = 16;
          mappings  = "0123456789abcdef";
          break;
        case CONVERSION_HEX_UPPERCASE:
          is_signed = false;
          base      = 16;
          mappings  = "0123456789ABCDEF";
          break;
        }

        if(is_signed)
        {
          intmax_t value;
          switch(length_modifier)
          {
          case LENGTH_CHAR:
          case LENGTH_SHORT:
          case LENGTH_INT:
            value = va_arg(args, int);
            break;
          case LENGTH_LONG:
            value = va_arg(args, long);
            break;
          case LENGTH_LONG_LONG:
            value = va_arg(args, long long);
            break;
          }
          debug_print_intmax(value, base, mappings);
        }
        else
        {
          uintmax_t value;
          switch(length_modifier)
          {
          case LENGTH_CHAR:
          case LENGTH_SHORT:
          case LENGTH_INT:
            value = va_arg(args, unsigned int);
            break;
          case LENGTH_LONG:
            value = va_arg(args, unsigned long);
            break;
          case LENGTH_LONG_LONG:
            value = va_arg(args, unsigned long long);
            break;
          }
          debug_print_uintmax(value, base, mappings);
        }
      }
    }
    else if(*p != '\0') // Try to parse a normal character
      debug_put(*p++);
  }

  va_end(args);
}
