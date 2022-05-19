#include "econsim.h"






int main(int argc, char**argv)
{
	t_econsim *sim;
	init_simulation(sim);
	while (sim->rounds > 0)
	{
		run_simulation(sim);
		print_results();			/* Print log of results from this round (Might be best to do this as the simulation runs, not at the end*/
		sim->rounds--;
	}
}


/* Attempt to perform production for each agent */
void    agent_actions(t_econsim *sim)
{
	int	i = 0;
	while (i < sim->num_agents)
	{
		perform_production(sim->agents[i]);
		update_price_beliefs(sim->agents[i]);
		create_offers(sim->agents[i]);
		post_bulletin();
		i++;
	}
}


/* Run a single round of the simulation */
void    run_simulation(t_econsim *sim)
{
	agent_actions(sim);
	run_auction(sim);
	//At some point need to handle "bankcruptcy" and creation of a new agent
}
