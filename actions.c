#include "econsim.h"

//All of these functions need to be run using data from an individual agent

/* Return the smaller of two values */
int	min(int n1, int n2)
{
	if (n1 > n2)
		return (n2);
	return (n1);
}

/* Return the larger of two values */
int	max(int n1, int n2)
{
	if (n1 < n2)
		return (n2);
	return (n1);
}

/*Singleton to fetch market data */
t_market	*get_market_data(void)
{
	static t_market	*market;

	if (!market)
		market = malloc(sizeof(t_market));
	return (market);
}



void    create_offers(t_agent *agent)
{
       //Attempt to generate offers
    if (inputs < theshold)
        generate_bids();
    if (outputs > threshold)
        generate_asks();
    
    //Return a null struct if no offers?
}


/* Check each commodity and attempt to generate bids if less than ideal amount is held*/
void	generate_bids(t_agent *agent)
{
	int	i = 0;
	
	/* Add logic to only create a bid for the item they have the least of if they are low on money */
	/* If they cannot produce a good, they should prioritize buying materials to produce */
	while(agent->inventory.current[i])
	{
		int commodity = agent->inventory.current[i]->key;
		int limit = agent->inventory.max[i]->value - agent->inventory.current[i]->value; 
		if (agent->inventory.current[i]->value < agent->inventory.ideal[i]->value && agent->inventory.current[i]->is_producer == false)
			create_bid(agent, commodity, limit);
		i++;
	}
}

/* Check each commodity produced by this agents and attempt to generate asks if any amount is held*/
void	generate_asks(t_agent *agent)
{
	int	i = 0;
	
	while(agent->inventory.current[i])
	{
		int commodity = agent->inventory.current[i]->key;
		int limit = agent->inventory.current[i]->value; 
		if (agent->inventory.current[i]->value > agent->inventory.ideal[i]->value && agent->inventory.current[i]->is_producer == true)
			create_ask(agent, commodity, limit);
		i++;
	}
}





/* Create an individual bid offer for a commodity */
void create_bid(t_agent *agent, int commodity, int limit)
{
	t_offer *bid;

	bid = malloc(sizeof(t_offer));
	bid->bid_ask = BID;
	bid->unit_price = get_price(agent, commodity, BID);
	int ideal_quantity = determine_purchase_quantity(agent, commodity, limit);
	bid->units = min(ideal_quantity, limit); /* Attempt to purchase the smaller of these two values */

	/* If agent does not have enough money to cover the bid, do not create and offer, or create and offer for an amount it can afford */
	/* This should probably have some logic to favour what they have the least of !!! */
	/* If the agent is desperate (low on money and low on commodities) maybe they should offer a premium to try and survive */
	if (bid->units * bid->unit_price > agent->wallet.balance)
	{
		while (bid->units * bid->unit_price > agent->wallet.balance)
		{
			if (bid->units == 0)
			{
				free(bid);
				return;
			}
			bid->units--;
		}
	}
	send_offer_to_auction(bid);		//Send the bid that was created to the auction house
}



//Determine how much of a commodity an agent wants to purchase
int	determine_purchase_quantity(t_agent *agent, int commodity, int limit, int bid_ask)
{
	float favourability = get_favourability(agent, commodity, BID);
	int purchase_quantity = favourability * limit;
	return (purchase_quantity);
}



//Get the current price for a commodity for the current agent
float   get_price(t_agent *agent, int commodity, int bid_ask)
{
	float need, lowest, highest, range, price;
	if (bid_ask == BID)				/* If offer is a BID, need is based on how empty their inventory is */
	{
		need = 1 - (agent->inventory.current[commodity]->value / agent->inventory.ideal[commodity]->value);
		get_price_data_agent(agent->price_history[commodity], &lowest, &highest, &range);
		price = lowest + (range * need);
	}
	else if (bid_ask == ASK)		/* If offer is an ASK, need is based on how full their inventory is */
	{
		need = (agent->inventory.current[commodity]->value / agent->inventory.max[commodity]->value);
		get_price_data_agent(agent->price_history[commodity], &lowest, &highest, &range);
		price = highest - (range * need);
		
	}
	return (price);
}


/* Get favourability value based on an agents price beliefs */
float	get_favourability(t_agent *agent, int commodity, int bid_ask)
{
	t_market *market = get_market_data();
	float lowest, highest, range;
	get_price_data_agent(agent->price_history[commodity], &lowest, &highest, &range);
	float favourability = (market->price_mean[commodity].value - lowest) / range;

	if (bid_ask == BID)
		return (1 - favourability);
	else
		return (favourability);
}

/* Get data about an agents observed price history */
void	get_price_data_agent(t_mapinv **price_history, float *lowest, float *highest, float *range)
{
	*highest = 0;
	*lowest = 3.40282e+038; //Float max
	*range = 0;
	int i = 0;
	while (price_history[i])
	{
		if (price_history[i]->value > *highest)
			*highest = price_history[i]->value;
		if (price_history[i]->value < *lowest)
			*lowest = price_history[i]->value;
		i++;
	}
	*range = *highest - *lowest;
}
//Price_history is actually the price beliefs of the agent. They do not maintain records of their trades in the current system


//Create an ask for selling a commodity
void create_ask(t_agent *agent, int commodity, int limit)
{
	t_offer *bid;

	bid = malloc(sizeof(t_offer));
	bid->bid_ask = ASK;
	bid->unit_price = get_price(agent, commodity, ASK);
	int ideal_quantity = determine_sale_quantity(agent, commodity, limit, ASK);
	bid->units = max(ideal_quantity, limit); /* Attempt to sell the larger of these two values */
	//Might want to always sell ideal_quanity as they want to sell less when prices are low?

	send_offer_to_auction(bid);		//Send the bid that was created to the auction house

}


//Determine how much of a commodity an agent wants to sell
int	determine_sale_quantity(t_agent *agent, int commodity, int limit, int bid_ask)
{
	float favourability = get_favourability(agent, commodity, ASK);
	int purchase_quantity = favourability * limit;
	return (purchase_quantity);
}



//Get the current price of a commodity based on price belief


//Limit is the most units that can be sold, or the least units that can be purchased??  I have it as most than can be purchased right now.
//Might need to change this!!
