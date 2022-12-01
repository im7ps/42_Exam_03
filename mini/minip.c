/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minip.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: sgerace <sgerace@student.42.fr>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2022/12/01 16:13:46 by sgerace           #+#    #+#             */
/*   Updated: 2022/12/01 19:40:26 by sgerace          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>

typedef struct s_zone
{
	int		width;
	int		height;
	char	background;
}	t_zone;

typedef struct s_circle
{
	char	type;
	float	w;
	float	h;
	float	r;
	char	c;
}	t_circle;

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

char	*ft_update_zone(FILE *file, t_zone *zone)
{
	int		count;
	char	*tmp;
	int		i;

	if ((count = fscanf(file, "%i %i %c\n", &zone->width, &zone->height, &zone->background) != 3))
		return (NULL);
	if (zone->width <= 0 || zone->width > 300 || zone->height <= 0 || zone->height > 300)
		return (NULL);
	tmp = (char *) malloc(sizeof(char) * (zone->width * zone->height));
	if (!tmp)
		return (NULL);
	i = 0;
	while (i < (zone->width * zone->height))
	{
		tmp[i] = zone->background;
		i++;
	}
	return (tmp);
}

int	it_belongs(float w, float h, t_circle *circle)
{
	float distance;

	distance = sqrt(((w - circle->w) * (w - circle->w)) + ((h - circle->h) * (h - circle->h)));
	if (distance <= circle->r)
	{
		if ((circle->r - distance) < 1.00000000) //la differenza fra il raggio effettivo del cerchio e il raggio descritto dal punto é minore di 1
		{
			return (2);
		}
		return (1);
	}
	return (0);
}

void	ft_define_one(t_zone *zone, char *pattern, t_circle *circle)
{
	int	w;
	int	h;
	int	belongs;

	h = 0;
	while (h < zone->height)
	{
		w = 0;
		while (w < zone->width)
		{
			belongs = it_belongs((float) w, (float) h, circle);
			if ((circle->type == 'C' && belongs) || (circle->type == 'c' && belongs == 2)) //se type == C belongs deve esistere (1 o 2 vanno bene), se type == c belongs deve essere per forza 2 (2 indica il bordo)
			{
				pattern[(h * zone->width) + w] = circle->c;
			}
			w++;
		}
		h++;
	}
}

int	ft_define_shapes(FILE *file, char *pattern, t_zone *zone, t_circle *circle)
{
	int		count;

	while ((count = fscanf(file, "%c %f %f %f %c\n", &circle->type, &circle->w, &circle->h, &circle->r, &circle->c)) == 5)
	{
		if (circle->r <= 0.00000000 || (circle->type != 'c' && circle->type != 'C'))
		{
			return (1);
		}
		ft_define_one(zone, pattern, circle);
	}
	if (count != -1)
	{
		return (1);
	}
	return (0);
}

int	ft_free_stuff(FILE *file, char *pattern, char *str)
{
	if (file)
		fclose(file);
	if (pattern)
		free(pattern);
	if (str)
		ft_str_error(str);
	return (1);
}

void	draw_circles(t_zone *zone, char *pattern)
{
	int i, j;
	i = 0;
	while (i < zone->width * zone->height)
	{
		j = 0;
		while (j < zone->width)
		{
			write(1, &pattern[i], 1);
			i++;
			j++;
		}
		write(1, "\n", 1);
	}
}

int	main(int argc, char **argv)
{
	FILE		*file;
	t_zone		zone;
	t_circle	circle;
	char		*pattern;

	zone.height = 0;
	zone.width = 0;
	zone.background = 0;

	file = fopen(argv[1], "r");
	if (file == NULL)
		return (ft_str_error("Error: Operation file corrupted\n"));
	if (argc != 2)
		return (ft_str_error("Error: argument\n"));
	if (!(pattern = ft_update_zone(file, &zone)))
		return (ft_free_stuff(file, pattern, "Error: Operation file corrupted!\n"));
	if (ft_define_shapes(file, pattern, &zone, &circle))
		return (ft_free_stuff(file, pattern, "Error: Operation file corrupted!!\n"));
	draw_circles(&zone, pattern);
	ft_free_stuff(file, pattern, NULL);
}
