#include "get_next_line.h"

static void	dest_fill(char *dest, char *src, size_t size)
{
	size_t	i;

	i = 0;
	if (src != NULL)
	{
		while (i < size)
		{
			dest[i] = src[i];
			i++;
		}
	}
	while (i < BUFFER_SIZE)
		dest[i++] = 0;
}

static char	*find_newline(char *s)
{
	size_t	i;

	i = 0;
	while (s[i] != '\0')
	{
		if (s[i] == '\n')
			return (s + i);
		i++;
	}
	return (NULL);
}

static char	*check_newline(t_sbuf **buf, char *temp,
	size_t* length)
{
	char	*new_line;

	new_line = find_newline(temp);
	if (new_line != NULL)
	{
		append_buf(*buf, temp, new_line - temp + 1);
		dest_fill(temp, new_line + 1, temp + BUFFER_SIZE - (new_line + 1));
		return (free_buf(buf, NO_FREE_CHARS, length));
	}
	else
	{
		append_buf(*buf, temp, BUFFER_SIZE);
		dest_fill(temp, NULL, BUFFER_SIZE);
		return (NULL);
	}
}

static char	*read_full_line(int fd, t_sbuf **buf,
	char *temp, size_t* length)
{
	char		*new_line;
	long long	chars_read;

	chars_read = BUFFER_SIZE;
	new_line = NULL;
	while ((new_line == NULL) && (chars_read == BUFFER_SIZE))
	{
		chars_read = READ(fd, temp, BUFFER_SIZE);
		new_line = check_newline(buf, temp, length);
	}
	if ((new_line == NULL) && (*buf != NULL))
		return (free_buf(buf, NO_FREE_CHARS, length));
	return (new_line);
}

char	*get_next_line(int fd, size_t* length)
{
	t_sbuf		*buf;
	static char	temp[BUFFER_SIZE + 1];
	long long	chars_read;
	char		*new_line;

	buf = init_buf();
	if (buf == NULL)
		return (NULL);
	new_line = check_newline(&buf, temp, length);
	if (new_line != NULL)
		return (new_line);
	chars_read = READ(fd, temp, BUFFER_SIZE);
	if (chars_read <= 0)
	{
		if ((chars_read == 0) && (buf->count != 0))
			return (free_buf(&buf, NO_FREE_CHARS, length));
		return (free_buf(&buf, FREE_CHARS, length));
	}
	new_line = check_newline(&buf, temp, length);
	if (new_line != NULL)
		return (new_line);
	if (chars_read == BUFFER_SIZE)
		return (read_full_line(fd, &buf, temp, length));
	return (free_buf(&buf, NO_FREE_CHARS, length));
}
