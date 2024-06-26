/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: vdarras <vdarras@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/23 12:15:47 by vdarras           #+#    #+#             */
/*   Updated: 2024/04/29 19:04:22 by vdarras          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

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
	size_t		len_line;
	char		*new_stash;

	count = 0;
	len_line = ft_strlen(line);
	i = ft_strlen(stash) - len_line;
	new_stash = malloc((i + 1) * sizeof(char));
	if (!new_stash)
	{
		free (stash);
		return (NULL);
	}
	while (stash[len_line + count])
	{
		new_stash[count] = stash[len_line + count];
		count++;
	}
	free(stash);
	new_stash[count] = '\0';
	return (new_stash);
}

char	*get_next_line(int fd)
{
	static char	*stash;
	char		*buffer;
	char		*line;

	if (fd < 0 || BUFFER_SIZE < 1)
		return (NULL);
	line = NULL;
	buffer = (char *)malloc(sizeof(char) * BUFFER_SIZE + 1);
	if (!buffer)
		return (NULL);
	stash = fill_stash(fd, buffer, stash);
	free(buffer);
	if (!stash)
		return (NULL);
	line = line_from_stash(stash, line);
	if (!line)
		return (free(stash), NULL);
	stash = get_new_stash(stash, line);
	if (!stash)
		return (NULL);
	if (*line == '\0')
	{
		return (free(line), free(stash), stash = NULL);
	}
	return (line);
}

// int main(int argc, char **argv)
// {
// 	char	*line;

// 	int i = 0;
// 	int fd = open("text.txt", O_RDONLY);
// 	if (argc == 2)
// 	{
// 		while (i < atoi(argv[1]))
// 		{
// 			line = get_next_line(fd);
// 			printf("Ligne %d : <%s", i + 1, line);
// 			free(line);
// 			i++;
// 		}
// 	}
// 	return (0);
// }