/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natrijau <natrijau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:05:09 by natrijau          #+#    #+#             */
/*   Updated: 2024/03/29 17:50:10 by natrijau         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philosophers.h"

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

void	check_dead(long int current_time, t_philosophers *philo, int flag)
{
	(void)current_time;
	// struct timeval my_time;
  	// long int check_time;
	// gettimeofday(&my_time, NULL);
	// check_time = ((my_time.tv_sec * 1000) + (my_time.tv_usec / 1000)) - current_time;
	if (flag == 0)
	{
		if (philo->start_dead >= philo->time_to_die
			|| philo->time_to_eat >= philo->time_to_die)
		{
			philo->alive = false;
			philo->end_time = philo->time_to_die / 1000;
		}
		else
			philo->start_dead = 0;
	}
	if (flag == 1)
	{
		philo->start_dead += philo->time_to_sleep + philo->time_to_eat;
		if (philo->start_dead >= philo->time_to_die
			|| philo->time_to_sleep >= philo->time_to_die)
		{
			philo->alive = false;
			philo->end_time = philo->time_to_die / 1000;
		}
	}	
}

void	ft_eating(t_philosophers *philo)
{
	struct timeval my_time;
  	long int current_time;
	pthread_mutex_lock(&philo->my_fork);
	pthread_mutex_lock(philo->next_fork);
	pthread_mutex_lock(&philo->print);
	gettimeofday(&my_time, NULL);
	current_time = (my_time.tv_sec * 1000) + (my_time.tv_usec / 1000);	
	printf("%ld\tLe philosophe %d take fork ! \n",current_time - philo->start_time,  philo->id_philosphers);
	printf("%ld\tLe philosophe %d take second fork ! \n",current_time - philo->start_time,  philo->id_philosphers);
	printf("%ld\tLe philosophe %d start eat \n",current_time - philo->start_time,  philo->id_philosphers);
	philo->number_of_times_each_philosopher_must_eat--;
	check_dead(current_time - philo->start_time, philo, 0);
	pthread_mutex_unlock(&philo->print);
	usleep(philo->time_to_eat);
	pthread_mutex_unlock(&philo->my_fork);
	pthread_mutex_unlock(philo->next_fork);
}

void	sleeping(t_philosophers *philo)
{
	struct timeval my_time;
  	long int current_time;
	
	pthread_mutex_lock(&philo->print);
	gettimeofday(&my_time, NULL);
	current_time = (my_time.tv_sec * 1000) + (my_time.tv_usec / 1000);
	printf("%ld\tLe philosophe %d sleep !\n",current_time - philo->start_time, philo->id_philosphers);
	check_dead(current_time - philo->start_time, philo, 1);
    usleep(philo->time_to_sleep);
	pthread_mutex_unlock(&philo->print);
}

void	thinking(t_philosophers *philo)
{	struct timeval my_time;
  	long int current_time;
	pthread_mutex_lock(&philo->print);
	gettimeofday(&my_time, NULL);
	current_time = (my_time.tv_sec * 1000) + (my_time.tv_usec / 1000);
	printf("%ld\tLe philosophe %d think !\n",(current_time - philo->start_time),  philo->id_philosphers);
	pthread_mutex_unlock(&philo->print);
}

void	*round_table(void *arg)
{
	t_philosophers	*philo = (t_philosophers *) arg;
	struct timeval start;
	gettimeofday(&start, NULL);
	philo->start_time = (start.tv_sec * 1000) + (start.tv_usec / 1000);
	while (1)
	{
		ft_eating(philo);
		if(philo->number_of_times_each_philosopher_must_eat == 0)
			break;
		sleeping(philo);
		thinking(philo);
	}
	pthread_exit(EXIT_SUCCESS);	
}

int	get_thread(t_data *data)
{
	unsigned int	num_fork;

	num_fork = data->data_philo->number_of_philosophers;
	while (num_fork > 0)
	{
		pthread_create(&data->data_philo[num_fork -1].thread_philo, NULL, round_table, &data->data_philo[num_fork - 1]);
		num_fork--;
	}
	while (num_fork < data->data_philo->number_of_philosophers)
	{
		if (data->data_philo[num_fork].alive == false)
		{
			pthread_mutex_lock(&data->data_philo[num_fork].print);
			printf("%ld\tLe philosophe %d est mort !\n", data->data_philo[num_fork].end_time, data->data_philo[num_fork].id_philosphers);
			pthread_mutex_unlock(&data->data_philo[num_fork].print);
			return(0);
		}
		num_fork++;
		if(num_fork == data->data_philo->number_of_philosophers && data->data_philo->number_of_times_each_philosopher_must_eat > 0)
			num_fork = 0;
	}	
	while (num_fork > 0)
	{
		pthread_join(data->data_philo[num_fork - 1].thread_philo, NULL);
		num_fork--;
	}
	return (0);
}

int	main(int ac, char **av)
{
	t_data data;
	if (!check_arg(ac, av))
		return (1);
	init_philo(&data, av);
	// print_test_init(&data);
	get_thread(&data);
	// free_all(&data);
	return (0);
}