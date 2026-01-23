#include "get_next_line.h"

t_sbuf	*init_buf(void)
{
	t_sbuf	*buf;

	if (BUFFER_SIZE == 0)
		return (NULL);
	buf = malloc(sizeof(t_sbuf));
	if (buf == NULL)
		return (NULL);
	buf->chars = NULL;
	buf->capacity = 0;
	buf->count = 0;
	return (buf);
}

static void	realloc_buf(t_sbuf *buf, size_t new_size)
{
	char	*new_chars;
	size_t	i;

	new_chars = malloc(new_size * sizeof(char));
	if (!new_chars)
		return ;
	i = 0;
	while (i < new_size)
		new_chars[i++] = 0;
	if (buf->chars)
	{
		i = 0;
		while (i < buf->count)
		{
			new_chars[i] = buf->chars[i];
			i++;
		}
		free(buf->chars);
	}
	buf->chars = new_chars;
	buf->capacity = new_size;
}

void	append_buf(t_sbuf *buf, const char *str, size_t size)
{
	size_t	i;

	if ((str == NULL) || (size <= 0))
		return ;
	while (buf->capacity < buf->count + size + 1)
	{
		if (buf->capacity == 0)
			realloc_buf(buf, BUFFER_SIZE);
		else
			realloc_buf(buf, buf->capacity * 2);
	}
	i = 0;
	while ((str[i] != '\0') && (i < size))
	{
		buf->chars[buf->count + i] = str[i];
		i++;
	}
	buf->count += i;
}

char	*free_buf(t_sbuf **buf, int free_chars, size_t* length)
{
	char	*temp;

	if (!buf)
		return (NULL);
	temp = NULL;
	if (*buf)
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
	return (temp);
}
