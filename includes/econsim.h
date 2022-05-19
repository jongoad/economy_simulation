#ifndef ECONSIM_H
# define ECONSIM_H



/* System includes */
# include <stdio.h>
# include <math.h>
# include <string.h>
# include <stdbool.h>
# include <limits.h>

enum bid_ask
{
	BID = 0,
	ASK = 1
};

enum commodity
{
    FOOD = 0,
    TOOLS = 1,
    WOOD = 2,
    METAL = 3
};

enum job
{
    FARMER = 0,
    BLACKSMITH = 1,
    LUMBERJACK = 2,
    MINER = 3
};

/* Inventory map */
typedef struct s_mapinv
{
	int		key;
	float   value;
	float	space_used;
	bool	is_producer;	/*If this is false, it means they are a consumer of this commodity */
}	t_mapinv;
/*	Use an enum to define values to each key base on commodity
	Create an array of these to hold data about all goods
*/

/* Inventory data */
typedef struct s_inventory
{
	int			max_space;
	int 		current_space;
	t_mapinv	**current;			/* Current amount of each commodity held */
	t_mapinv	**ideal;			/* Ideal amount of each commodity to hold */
	t_mapinv	**max;				/* Maximum amount of each commodity to hold */
}	t_inventory;

/* Wallet data */
typedef struct s_wallet
{
	float	balance;
}   t_wallet;

/* Agent data */
typedef struct s_agent
{
	int			agent_id;			/* Unique agent id */
	int			job;				/* Job value, reference using enum */
	t_inventory	inventory;
	t_wallet	wallet;
	t_mapinv	volume_agent;		/* Volume of each commodity traded as a result of this agents offers last round */
	t_mapinv	***price_history;	/* Array of price history arrays */
}   t_agent;

/* Public market data */
typedef struct s_market
{
	t_mapinv	*supply;
	t_mapinv	*demand;
	t_mapinv    *volume;            /* Trade volume for each commodity in the last round */
    t_mapinv    *price_mean;         /* Average price for each commodity in last round */
    t_mapinv    *price_max;			/* Max price for each commodity in the last round */
	t_mapinv	*price_min;			/* Min price for each commodity in the last round */
}   t_market;

/* Market bulletin data */
typedef struct s_market_news
{
    t_mapinv    *volume;            /* Trade volume for each commodity in the last round */
    t_mapinv    *price_mean;         /* Average price for each commodity in last round */
    t_mapinv    *price_max;			/* Max price for each commodity in the last round */
	t_mapinv	*price_min;			/* Min price for each commodity in the last round */
}   t_market_news;


/* Single offer data */
typedef struct s_offer
{
	int		agent_id;
	int		commodity;
	int		units;
	float	unit_price;
	int		bid_ask;

}	t_offer;

/* Auction house data */
typedef struct s_auctions
{
	t_offer		**offers;		/* Array of offers for round */
	t_market	stats;			/* Current market stats */
}	t_auctions;

/* Simulation data struct */
typedef struct s_econsim
{
	int		rounds;             /* Number of rounds */
	int		num_agents;			/* Number of agents */
	t_agent	**agents;			/* Array of agents */
								/* Array of production functions */
}   t_econsim;



#endif