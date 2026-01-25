#include "get_next_line.h"
#include <stddef.h>
#include <string.h>

sbuf *init_buf(void)
{
	sbuf* buf = malloc(sizeof(sbuf));
	if (buf == NULL) return NULL;

	buf->chars = NULL;
	buf->capacity = 0;
	buf->count = 0;
	return buf;
}

static void realloc_buf(sbuf *buf, size_t new_size)
{
	char* new_chars = malloc(new_size * sizeof(char));
	if (new_chars == NULL) return;

	memset(new_chars, '\0', new_size);
	if (buf->chars != NULL)
	{
		memcpy(new_chars, buf->chars, buf->count);
		free(buf->chars);
	}

	buf->chars = new_chars;
	buf->capacity = new_size;
}

void append_buf(sbuf *buf, const char *str, size_t size)
{
	if ((str == NULL) || (size <= 0))
		return;

	while (buf->capacity < buf->count + size + 1)
	{
		if (buf->capacity == 0)
			realloc_buf(buf, BUFFER_SIZE);
		else
			realloc_buf(buf, buf->capacity * 2);
	}

	size_t i = 0;
	while ((str[i] != '\0') && (i < size))
	{
		buf->chars[buf->count + i] = str[i];
		i++;
	}
	buf->count += i;
}

char* free_buf(sbuf **buf, bool free_chars, size_t* length)
{
	if (buf == NULL) return (NULL);

	char* temp = NULL;
	if (*buf != NULL)
	{
		if (free_chars)
		{
			free((*buf)->chars);
			if (length != NULL)
				*length = 0;
		}
		else
		{
			temp = (*buf)->chars;
			if (length != NULL)
				*length = (*buf)->count;
		}
	}

	free(*buf);
	*buf = NULL;
	return temp;
}