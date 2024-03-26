/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natrijau <natrijau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:05:09 by natrijau          #+#    #+#             */
/*   Updated: 2024/03/22 14:04:26 by natrijau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"
// float time_diff(struct timeval *start, struct timeval *end) {
//   return (end->tv_sec - start->tv_sec) + 1e-6 * (end->tv_usec - start->tv_usec);
// }

bool	its_integer(char *str)
{
	int	i;

	i = 0;
	if (!str)
		return (false);	
	while (str[i])
	{
		if (str[i] > '9' || str[i] < '0')
			return (false);
		i++;
	}
	return (true);
}

bool	all_positiv_num(char **av)
{
	if (ft_atoi(av[1]) < 0
		|| ft_atoi(av[2]) < 0
			|| ft_atoi(av[3]) < 0
				|| ft_atoi(av[4]) < 0)
	return (false);
	if (av[5] && ft_atoi(av[5]) <= 0)
		return (false);
	return (true);
}

void	init_philo(t_data *data, char **av)
{
	int	i;
	int	number_philo;
	pthread_mutex_t	*mutex;
	number_philo = ft_atoi(av[1]);
	mutex = malloc(sizeof(pthread_mutex_t) * number_philo);
	data->data_philo = malloc(sizeof(t_philosophers) * number_philo);
	i = 0;
	while (i < number_philo)
	{
		data->data_philo[i].number_of_philosophers = ft_atoi(av[1]);
		data->data_philo[i].time_to_die = ft_atoi(av[2]);	
		data->data_philo[i].time_to_eat = ft_atoi(av[3]);	
		data->data_philo[i].time_to_sleep = ft_atoi(av[4]);
		if (av[5])
			data->data_philo[i].number_of_times_each_philosopher_must_eat = ft_atoi(av[5]);
		data->data_philo[i].id_philosphers = i;
		data->data_philo[i].my_fork = mutex[i];
		data->data_philo[i].next_fork = &data->data_philo[i + 1].my_fork;
		i++;
	}
	data->data_philo[i - 1].next_fork = &data->data_philo[0].my_fork;
	free(mutex);
}

bool	check_arg(int ac, char **av)
{
	int	i;

	i = 1;
	if (ac != 5 && ac != 6)
	{
		printf("Invalid number of arguments !\n");
		return (false);
	}
	while (av[i])
	{
		if (!its_integer(av[i]))
		{
			printf("One of the arguments is invalid !\n");
			return (false);			
		}
		i++;
	}	
	if (!all_positiv_num(av))
	{
		printf("One of the arguments is invalid !\n");
		return(false);
	}
	return(true);
}	

void	ft_eating(t_data *data)
{
	pthread_mutex_lock(&data->mutex);
	
	pthread_mutex_unlock(&data->mutex);
}

void	sleeping(t_philosophers *philo)
{
	unsigned int mSeconds = philo->time_to_sleep;
    int returnCode;
    returnCode = usleep(mSeconds);
	printf("temps ecoule en milisec -> %dmicrosecondes\nEn sec -> %dsec", mSeconds, mSeconds / 1000000 );
	printf("\nusleep result -> %d\n", returnCode);		
}

void	thinking(t_philosophers *philo)
{
	(void) philo;
	// time
	printf("Thinking philo = %d\n", philo->id_philosphers);
}

void	star_time(void)
{
	struct	timeval	start;
	struct	timeval	end;
	gettimeofday(, NULL);
}

void	*round_table(void *arg)
{
	t_philosophers	*philo = (t_philosophers *) arg;
	start_time(0, 0);
	while (1)
	{
		pthread_mutex_lock(&philo->my_fork);
		pthread_mutex_lock(philo->next_fork);
		printf("J'ai pris les fourchettes ! : id_philo %d\n", philo->id_philosphers);
		philo->number_of_times_each_philosopher_must_eat--;
		pthread_mutex_unlock(philo->next_fork);
		pthread_mutex_unlock(&philo->my_fork);
		if(philo->number_of_times_each_philosopher_must_eat == 0)
			break;
		sleeping(philo);
		usleep(philo->time_to_sleep);
		thinking(philo);
	}
	pthread_exit(EXIT_SUCCESS);	
}

void	get_thread(t_data *data)
{
	unsigned int	num_fork;

	num_fork = data->data_philo->number_of_philosophers;
	data->id_fork = malloc(sizeof(int) * num_fork);
	while (num_fork > 0)
	{
		pthread_create(&data->data_philo[num_fork -1].thread_philo, NULL, round_table, &data->data_philo[num_fork - 1]);
		num_fork--;
	}
	while (num_fork < data->data_philo->number_of_philosophers)
	{
		pthread_join(data->data_philo[num_fork].thread_philo, NULL);
		num_fork++;
	}
}

void	init_mutex(pthread_mutex_t *take_fork, t_data *data)
{
	int	num_fork;
	int	i;

	i = 0;
	num_fork = data->data_philo->number_of_philosophers;
	while (i < num_fork)
	{
		pthread_mutex_init(&take_fork[i], NULL);
		i++;
	}
}

int	main(int ac, char **av)
{
	t_data data;
	if (!check_arg(ac, av))
		return (1);
	init_philo(&data, av);
	get_thread(&data);
	free_all(&data);
	return (0);
}