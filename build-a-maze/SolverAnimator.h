#ifndef SOLVER_ANIMATOR
#define SOLVER_ANIMATOR

#include <vector>
#include "MazeSolver.h"
#include "Renderer.h"

class SolverAnimator
{
public:
	SolverAnimator(sf::RenderWindow & t_window, std::vector<MazeSolver*>& t_solvers);

	void update();

	void draw();

	void startAnimating();
	const bool isAnimating() const;

private:

	sf::RenderWindow & m_windowRef;
	std::vector<MazeSolver*> & m_solversRef;

	bool m_animating;
};

#endif // !SOLVER_ANIMATOR