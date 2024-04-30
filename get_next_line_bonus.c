/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line_bonus.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 12:15:47 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/29 17:31:57 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line_bonus.h"

static char	*fill_stash(int fd, char *buffer, char *stash)
{
	ssize_t		count_read;
	char		*temp;

	count_read = 1;
	if (!stash)
		stash = ft_strdup("");
	if (!stash)
		return (NULL);
	while (count_read > 0)
	{
		if (ft_strchr(stash, '\n'))
			break ;
		count_read = read(fd, buffer, BUFFER_SIZE);
		if (count_read == -1)
			return (free(stash), NULL);
		buffer[count_read] = '\0';
		temp = ft_strjoin(stash, buffer);
		free(stash);
		if (!temp)
			return (NULL);
		stash = temp;
	}
	return (stash);
}

static char	*line_from_stash(char *stash, char *line)
{
	size_t	len_end_of_line;
	size_t	i;

	len_end_of_line = 0;
	i = 0;
	while (stash[len_end_of_line] != '\n' && stash[len_end_of_line])
		len_end_of_line++;
	if (stash[len_end_of_line] == '\n')
		len_end_of_line++;
	line = malloc(sizeof(char) * len_end_of_line + 1);
	if (!line)
		return (NULL);
	while (i < len_end_of_line)
	{
		line[i] = stash[i];
		i++;
	}
	line[i] = '\0';
	return (line);
}

static char	*get_new_stash(char	*stash, char *line)
{
	size_t		i;
	size_t		count;
	char		*new_stash;

	count = 0;
	i = ft_strlen(stash) - ft_strlen(line);
	new_stash = malloc((i + 1) * sizeof(char));
	if (!new_stash)
	{
		free (stash);
		return (NULL);
	}
	while (stash[ft_strlen(line) + count])
	{
		new_stash[count] = stash[ft_strlen(line) + count];
		count++;
	}
	free(stash);
	new_stash[count] = '\0';
	return (new_stash);
}

char	*get_next_line(int fd)
{
	static char	*stash[MAX_FD];
	char		*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	line = NULL;
	buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	stash[fd] = fill_stash(fd, buffer, stash[fd]);
	free(buffer);
	if (!stash[fd])
		return (NULL);
	line = line_from_stash(stash[fd], line);
	if (!line)
		return (free(stash[fd]), NULL);
	stash[fd] = get_new_stash(stash[fd], line);
	if (!stash[fd])
		return (NULL);
	if (*line == '\0')
	{
		return (free(line), free(stash[fd]), stash[fd] = NULL);
	}
	return (line);
}
