/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philosophers.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: natrijau <natrijau@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 14:05:09 by natrijau          #+#    #+#             */
/*   Updated: 2024/03/28 13:47:18 by natrijau         ###   ########.fr       */
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

struct {
	bool is_taken;
	pthread_mutex_t lock;
} t_protected_fork;


void	ft_eating(t_philosophers *philo, t_data *data)
{
	struct timeval my_time;
  	long int current_time;
	// // usleep(3000000);
	// // printf("start = %ld\n", end.tv_sec - start.tv_sec);
	gettimeofday(&my_time, NULL);
	// printf("Le philosophe %d pense !\n", philo->id_philosphers);
	pthread_mutex_lock(&philo->my_fork);
	pthread_mutex_lock(philo->next_fork);
	usleep(philo->time_to_eat * 1000);
	current_time = (my_time.tv_sec * 1000) + (my_time.tv_usec / 1000);	
	printf("%ld\tLe philosophe %d prend la 1ere fourchette ! \n",current_time - data->start_time,  philo->id_philosphers);
	printf("%ld\tLe philosophe %d prend la deuxieme fourchettes ! \n",current_time - data->start_time,  philo->id_philosphers);
	printf("%ld\tLe philosophe %d mange \n",current_time - data->start_time,  philo->id_philosphers);
	philo->number_of_times_each_philosopher_must_eat--;
	pthread_mutex_unlock(&philo->my_fork);
	pthread_mutex_unlock(philo->next_fork);
	// printf("Le philosophe %d dort !\n", philo->id_philosphers);
	// usleep(philo->time_to_sleep * 1000);
	// printf("Le philosophe %d pense !\n", philo->id_philosphers);
	// usleep(100);
}

void	sleeping(t_philosophers *philo, t_data *data)
{
	struct timeval my_time;
  	long int current_time;
	gettimeofday(&my_time, NULL);
    usleep(philo->time_to_sleep);
	current_time = (my_time.tv_sec * 1000) + (my_time.tv_usec / 1000);
	printf("%ld\tLe philosophe %d dort !\n",current_time - data->start_time, philo->id_philosphers);
}

void	thinking(t_philosophers *philo, t_data *data)
{	struct timeval my_time;
  	long int current_time;
	gettimeofday(&my_time, NULL);
	usleep(100);
	current_time = (my_time.tv_sec * 1000) + (my_time.tv_usec / 1000);
	printf("%ld\tLe philosophe %d pense !\n",(current_time - data->start_time),  philo->id_philosphers);
}

float time_diff(struct timeval *start, struct timeval *end) {
  return ((end->tv_sec - start->tv_sec) + 1e-6 * (end->tv_usec - start->tv_usec));
}

void	*round_table(void *arg)
{
	t_data	*data = (t_data *) arg;
	struct timeval start;
	gettimeofday(&start, NULL);
	data->start_time = (start.tv_sec * 1000) + (start.tv_usec / 1000);
	printf("depart timer %ld\n", data->start_time);
	while (1)
	{
		ft_eating(data->data_philo, data);
		if(data->data_philo->number_of_times_each_philosopher_must_eat == 0)
			break;
		sleeping(data->data_philo, data);
		thinking(data->data_philo, data);
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
		pthread_create(&data->data_philo[num_fork -1].thread_philo, NULL, round_table, data);
		num_fork--;
	}
	while (num_fork < data->data_philo->number_of_philosophers)
	{
		pthread_join(data->data_philo[num_fork].thread_philo, NULL);
		num_fork++;
	}
}

int	main(int ac, char **av)
{
	t_data data;
	if (!check_arg(ac, av))
		return (1);
	init_philo(&data, av);
	// print_test_init(&data);
	get_thread(&data);
	free_all(&data);
	return (0);
}