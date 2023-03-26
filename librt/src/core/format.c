#include <rt/core/format.h>

#include <rt/core/string.h>
#include <stdbool.h>

struct rule
{
  const char *str;
  unsigned    res;
};

static inline bool consume(const char **format, const char *str)
{
  size_t len = strlen(str);
  if(strncmp(*format, str, len) == 0)
  {
    *format += len;
    return true;
  }
  return false;
}

static inline bool match(const char **format, size_t n, const struct rule rules[n], unsigned *res)
{
  for(size_t i=0; i<n; ++i)
    if(consume(format, rules[i].str))
    {
      *res = rules[i].res;
      return true;
    }

  return false;
}

enum type {
  TYPE_INT,
  TYPE_LONG_INT,
  TYPE_LONG_LONG_INT,
  TYPE_UNSIGNED,
  TYPE_LONG_UNSIGNED,
  TYPE_LONG_LONG_UNSIGNED,
  TYPE_STR,
};

enum conv {
  CONV_BIN,
  CONV_OCT,
  CONV_DEC,
  CONV_LOWER_HEX,
  CONV_UPPER_HEX,
  CONV_NONE,
};

#define TOKEN_SPECIAL (char)-1
struct token
{
  char c;
  unsigned type, conv;
};

static inline const char *next_token(const char *format, struct token *token)
{
  static const struct rule type_rules[] = {
    { "s",   TYPE_STR },
    { "llu", TYPE_LONG_LONG_UNSIGNED },
    { "lu",  TYPE_LONG_UNSIGNED      },
    { "u",   TYPE_UNSIGNED           },
    { "ll",  TYPE_LONG_LONG_INT   },
    { "l",   TYPE_LONG_INT        },
    { "",    TYPE_INT             },
  };

  static const struct rule conv_rules[] = {
    { "b", CONV_BIN       },
    { "o", CONV_OCT       },
    { "d", CONV_DEC       },
    { "x", CONV_LOWER_HEX },
    { "X", CONV_UPPER_HEX },
    { "",  CONV_NONE      },
  };

  if(consume(&format, "{"))
  {
    token->c    = TOKEN_SPECIAL;
    token->conv = CONV_DEC;
    token->type = TYPE_INT;

    if(match(&format, sizeof conv_rules / sizeof conv_rules[0], conv_rules, &token->conv))
      consume(&format, ":");

    match(&format, sizeof type_rules / sizeof type_rules[0], type_rules, &token->type);
    consume(&format, "}");
  }
  else
    token->c = *format++;

  return format;
}

// TODO: Consider adding real file abstraction
struct output
{
  char  *buf;
  size_t i, n;
};

static inline void putc(struct output *output, char c)
{
  if(output->i == output->n)
    return;

  output->buf[output->i++] = c;
}

static inline void puts(struct output *output, const char *s)
{
  for(; *s; ++s)
    putc(output, *s);
}

static inline void putu(struct output *output, enum conv conv, unsigned long long u)
{
  if(u == 0)
    putc(output, '0');

  char buf[64]; // Support at most 64-bit unsigned integer in binary format with null terminator
  size_t i = 64;

  unsigned    base;
  const char *mapping;
  switch(conv)
  {
  case CONV_BIN:                  base = 2;  mapping = "01";               break;
  case CONV_OCT:                  base = 8;  mapping = "01234567";         break;
  case CONV_DEC:         default: base = 10; mapping = "0123456789";       break;
  case CONV_LOWER_HEX:            base = 16; mapping = "0122456789abcdef"; break;
  case CONV_UPPER_HEX:            base = 16; mapping = "0122456789ABCDEF"; break;
  }

  buf[--i] = '\0';
  for(; u; u /= base)
    buf[--i] = mapping[u % base];

  puts(output, &buf[i]);
}

static inline void puti(struct output *output, enum conv conv, long long i)
{
  if(i == 0)
    putc(output, '0');

  if(i<0)
  {
    putc(output, '-');
    putu(output, conv, -i);
  }
  else
    putu(output, conv, i);
}

void format(char *buf, size_t n, const char *format, ...)
{
  va_list ap;
  va_start(ap, format);
  vformat(buf, n, format, ap);
  va_end(ap);
}

void vformat(char *buf, size_t n, const char *format, va_list ap)
{
  struct output output = { buf, 0, n };
  while(*format)
  {
    struct token token;
    format = next_token(format, &token);
    if(token.c == TOKEN_SPECIAL)
    {
      union {
        long long int       i;
        long long unsigned  u;
        const char         *s;
      } val;

      switch(token.type) {
        case TYPE_INT:                val.i = va_arg(ap, int);                    break;
        case TYPE_LONG_INT:           val.i = va_arg(ap, long int);               break;
        case TYPE_LONG_LONG_INT:      val.i = va_arg(ap, long long int);          break;
        case TYPE_UNSIGNED:           val.u = va_arg(ap, unsigned int);           break;
        case TYPE_LONG_UNSIGNED:      val.u = va_arg(ap, unsigned long int);      break;
        case TYPE_LONG_LONG_UNSIGNED: val.u = va_arg(ap, unsigned long long int); break;
        case TYPE_STR:                val.s = va_arg(ap, const char *);           break;
      }

      switch(token.type) {
        case TYPE_INT:
        case TYPE_LONG_INT:
        case TYPE_LONG_LONG_INT:
          puti(&output, token.conv, val.i);
          break;
        case TYPE_UNSIGNED:
        case TYPE_LONG_UNSIGNED:
        case TYPE_LONG_LONG_UNSIGNED:
          putu(&output, token.conv, val.u);
          break;
        case TYPE_STR:
          puts(&output, val.s);
          break;
      }
    }
    else
      putc(&output, token.c);
  }
  putc(&output, '\0');
  buf[n-1] == '\0';
}

