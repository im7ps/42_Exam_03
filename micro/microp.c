/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   microp.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgerace <sgerace@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/05 11:17:47 by sgerace           #+#    #+#             */
/*   Updated: 2022/12/05 20:47:52 by sgerace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>

typedef struct s_zone
{
	int		width;
	int		height;
	char	background;
} t_zone;

typedef struct s_square
{
	char	type;
	float	x;
	float	y;
	float	width;
	float	height;
	char	c;
}	t_square;

int	ft_strlen(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		i++;
	}
	return (i);
}

int	ft_str_error(char *str)
{
	int	i;

	i = ft_strlen(str);
	write(1, str, i);
	return (1);
}

int	ft_get_zone(FILE *file,t_zone *zone)
{
	int		count;

	count = fscanf(file, "%i %i %c\n", &zone->width, &zone->height, &zone->background);
	if (count != 3)
		return (1);
	if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
		return (1);
	if (count == -1)
		return (1);
	return (0);
}

int	ft_clear_stuff(FILE *file, char *str, char *pattern)
{
	if (file)
		fclose(file);
	if (str)
		return (ft_str_error(str));
	if (pattern)
		free(pattern);
	return (1);
}

char	*ft_upload_background(t_zone *zone)
{
	int		i;
	char	*tmp;

	i = 0;
	tmp = (char *) malloc (sizeof(char) * (zone->width * zone->height));
	if (!tmp)
		return (NULL);
	while (i < (zone->width * zone->height))
	{
		tmp[i] = zone->background;
		i++;
	}
	return (tmp);
}

int	ft_is_rec(float x, float y, t_square *square)
{
	if ((x < square->x || x > square->x + square->width) || (y < square->y || y > square->height + square->y))
		return (0);
	if (((x < square->x + 1.00000000) || (x > square->x + square->width - 1.00000000)) || ((y < square->y + 1.00000000) || (y > square->y + square->height - 1.00000000)))
		return (2);
	return (1);
}

void	draw_shape(char *pattern, t_square *square, t_zone *zone)
{
	int	i;
	int	j;
	int	is;

	i = 0;
	while (i < zone->height)
	{
		j = 0;
		while (j < zone->width)
		{
			is = ft_is_rec(j, i, square);
			if ((square->type == 'R' && is) || (square->type == 'r' && is == 2))
			{
				pattern[(i * zone->width) + j] = square->c;
			}
			j++;
		}
		i++;
	}
}

int	ft_define_shapes(FILE *file, char *pattern, t_zone *zone, t_square *square)
{
	int	count;

	while ((count = fscanf(file, "%c %f %f %f %f %c\n", &square->type, &square->x, &square->y, &square->width, &square->height, &square->c)) == 6)
	{
		if ((square->type != 'R' && square->type != 'r') || square->width < 0.00000000 || square->height < 0.00000000)
			return (1);
		draw_shape(pattern, square, zone);
	}
	if (count != -1)
		return (1);
	return (0);
}

void	draw_drawing(char *drawing, t_zone *zone)
{
	int	i;

	i = 0;
	while (i < zone->height)
	{
		write(1, drawing + (i * zone->width), zone->width);
		write(1, "\n", 1);
		i++;
	}
}

int	main(int argc, char **argv)
{
	t_zone		zone;
	t_square	square;
	FILE	*file;
	char	*pattern;
	
	zone.width = 0;
	zone.height = 0;
	zone.background = 0;

	square.c = 0;
	square.x = 0;
	square.y = 0;
	square.height = 0;
	square.width = 0;
	square.type = 0;

	if (argc != 2)
	{
		return(ft_str_error("Error: argument\n"));
	}
	file = fopen(argv[1], "r");
	if (file == NULL)
	{
		return(ft_str_error("Error: Operation file corrupted\n"));
	}
	if (ft_get_zone(file, &zone))
	{
		return(ft_clear_stuff(file, "Error: Operation file corrupted\n", NULL));
	}
	if (!(pattern = ft_upload_background(&zone)))
	{
		return (ft_clear_stuff(file, "Error: Malloc failed\n", pattern));
	}
	if(ft_define_shapes(file, pattern, &zone, &square))
	{
		return (ft_clear_stuff(file, "Error: Operation file corrupted\n", pattern));
	}
	draw_drawing(pattern, &zone);
	ft_clear_stuff(file, NULL, pattern);
	return (0);
}