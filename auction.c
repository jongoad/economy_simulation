#include "econsim.h"


/* Randomize bids before resolving so that order priority is negated */
void    shuffle_bids(t_econsim *sim)
{

}

/* Match bids and set success or failure for each bid */
void    resolve_bids(t_econsim *sim)
{

}

/* Update current market information */
void    update_market(t_econsim *sim)
{
	
}

/* Run a single round of auctions */
void    run_auction(t_econsim *sim)
{
	shuffle_bids(sim);
	resolve_bids(sim);
}


/* Information available to all agents

At the end of each round all agents are given the following information:
	- Trade volume for each commodity
	- Average price of all trades within each commodity
	- The highest and lowest prices offered for each commodity
	- Quantity of each commodity traded as a result of their offers

Information that is always available from the auction house, based on a defined window of time.
	- Average price for each commodity
	- Average quantity of each commodity offered for sale (supply)
	- Average quanity of each commodity asked to be purchased (demand)

*/