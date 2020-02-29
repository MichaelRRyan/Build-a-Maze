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

	void startAnimatingIn();
	void animateIn(MazeSolver * t_solver, float t_modifier);
	void startAnimatingOut();
	const bool isAnimating() const;

	void setTimeModifier(float t_modifier);

private:

	void animateIn(MazeSolver* t_solver);
	void animateOut(MazeSolver* t_solver);

	void moveSolver(MazeSolver* t_solver, Direction t_direction);

	sf::RenderWindow & m_windowRef;
	std::vector<MazeSolver*> & m_solversRef;

	float m_timeModifer;

	bool m_animating;
};

#endif // !SOLVER_ANIMATOR