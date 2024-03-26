#ifndef		PHILOSOPHERS_H
# define	PHILOSOPHERS_H

/*printf*/
# include <stdio.h>

/*usleep*/
# include <unistd.h>

/*gettimeofday*/
# include <sys/time.h>

/*bool*/
# include <stdbool.h>

/*malloc*/
# include <stdlib.h>

/*thread*/
# include <pthread.h>

typedef struct s_philosophers
{
	unsigned int	number_of_philosophers;
	unsigned int	time_to_die;
	unsigned int	time_to_eat;
	unsigned int	time_to_sleep;
	unsigned int	number_of_times_each_philosopher_must_eat;
	unsigned int	id_philosphers;
	bool			die;
	pthread_t		thread_philo;
	pthread_mutex_t	my_fork;
	pthread_mutex_t	*next_fork;
	// pthread_mutex_t	*eat;
	// pthread_mutex_t	*dead;
	// pthread_mutex_t	*print;
} t_philosophers;

typedef struct s_data
{
	int				dead_id;
	int				*id_fork;
	// pthread_t		thread;
	pthread_mutex_t	mutex;
	pthread_mutex_t	*eat;
	pthread_mutex_t	*dead;
	pthread_mutex_t	*print;
	t_philosophers	*data_philo;
} t_data;

/*philosophers_utils.c*/
unsigned int	ft_atoi(const char *nptr);
void			free_all(t_data *data);


#endif 