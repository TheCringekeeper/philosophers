/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ankhabar <ankhabar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/03/18 20:55:05 by ankhabar          #+#    #+#             */
/*   Updated: 2023/03/28 12:40:43 by ankhabar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

/* input error checks for the following errors:
** If the number of philosophers is less than or equal to zero
** If the must_eat parameter is negative when provided as input argument
** If the time to die, eat or sleep is less than or equal to zero
** If the simulation start time is zero
** If the mutexes have not been initialized (if the pointer is NULL).*/
static bool	input_error(t_data *data, int ac)
{
	return (data->philosophers <= 0 || (ac == 6 && data->must_eat < 0)
		|| data->time_to_die <= 0 || data->time_to_eat <= 0
		|| data->sim_start == 0 || data->time_to_sleep <= 0
		|| data->mutexes == NULL);
}

/* input_scanner function takes command-line arguments
** and stores them in the data structure */
static t_data	*input_scanner(int ac, char *av[])
{
	t_data	*data;

	data = malloc(sizeof(t_data));
	if (data == NULL)
		return (NULL);
	data->must_eat = -1;
	data->mutexes = NULL;
	data->should_exit = false;
	data->sim_start = get_time();
	data->philosophers = ft_atoi(av[1]);
	data->time_to_die = ft_atoi(av[2]);
	data->time_to_eat = ft_atoi(av[3]);
	data->time_to_sleep = ft_atoi(av[4]);
	if (ac == 6)
		data->must_eat = ft_atoi(av[5]);
	data->mutexes = init_mutexes();
	if (input_error(data, ac))
		return (free(data->mutexes), free(data), NULL);
	return (data);
}

/*init_mutexes function initializes the mutex locks */
t_mutex	*init_mutexes(void)
{
	int		index;
	t_mutex	*mutexes;

	index = 0;
	mutexes = NULL;
	mutexes = malloc(sizeof(t_mutex) * M_NUM);
	if (mutexes == NULL)
		return (NULL);
	while (index < M_NUM)
		pthread_mutex_init(&mutexes[index++], 0);
	return (mutexes);
}

/* init_philos function initializes each philosopher's attributes */
static bool	init_philos(t_philo *philos, t_data *data)
{
	int		i;
	t_mutex	*fork;

	i = 0;
	fork = malloc(sizeof(t_mutex) * data->philosophers);
	if (fork == NULL)
		return (EXIT_FAILURE);
	while (i < data->philosophers)
		pthread_mutex_init(&fork[i++], 0);
	i = 0;
	while (i < data->philosophers)
	{
		philos[i].id = i + 1;
		philos[i].data = data;
		philos[i].forks = fork;
		philos[i].eat_times = 0;
		if (data->must_eat == -1)
			philos[i].eat_times = -1;
		philos[i].left_fork = i;
		philos[i].right_fork = (i + 1);
		if (i == (data->philosophers) - 1)
			philos[i].right_fork = 0;
		philos[i++].last_eat = get_time();
	}
	return (EXIT_SUCCESS);
}

/* init_struct initializes the data structure and philosopher's threads */
t_philo	*init_struct(int ac, char *av[])
{
	t_philo	*philos;
	t_data	*data;

	data = input_scanner(ac, av);
	if (data == NULL)
		return (NULL);
	philos = malloc(sizeof(t_philo) * data->philosophers);
	if (philos == NULL)
		return (NULL);
	philos->data = data;
	if (philos->data == NULL)
		return (NULL);
	if (init_philos(philos, philos->data) == EXIT_FAILURE)
		return (NULL);
	return (philos);
}
