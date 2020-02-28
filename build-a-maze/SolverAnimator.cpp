#include "SolverAnimator.h"

///////////////////////////////////////////////////////////////////////////
SolverAnimator::SolverAnimator(sf::RenderWindow& t_window, std::vector<MazeSolver*>& t_solvers) :
	m_windowRef{ t_window },
	m_solversRef{ t_solvers },
	m_animating{ false },
	m_timeModifer{ 1.0f }
{
}

///////////////////////////////////////////////////////////////////////////
void SolverAnimator::update()
{
	// if the solvers are animating
	if (m_animating)
	{
		// Loop all solvers
		for (MazeSolver* solver : m_solversRef)
		{
			// If the individual solver is still animating
			if (solver->isAnimatingIn())
			{
				animateIn(solver);
			}
			else if (solver->isAnimatingOut())
			{
				animateOut(solver);
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////
void SolverAnimator::draw()
{
	for (MazeSolver * solver : m_solversRef)
	{
		solver->draw(m_windowRef);
	}
}

///////////////////////////////////////////////////////////////////////////
void SolverAnimator::startAnimatingIn()
{
	m_animating = true;

	int i = 0;
	for (MazeSolver* solver : m_solversRef)
	{
		solver->setMovementSpeed(20);
		solver->setPos(1, -8 - (i * 3));
		solver->setAnimatingIn(true);
		solver->setMovementDirection(Direction::East);
		solver->animate();
		i++;
	}
}

///////////////////////////////////////////////////////////////////////////
void SolverAnimator::startAnimatingOut()
{
	m_animating = true;

	for (MazeSolver* solver : m_solversRef)
	{
		solver->setMovementSpeed(20);
		solver->setAnimatingOut(true);
		solver->setMovementDirection(Direction::West);
		solver->animate();
	}
}

///////////////////////////////////////////////////////////////////////////
const bool SolverAnimator::isAnimating() const
{
	return m_animating;
}

///////////////////////////////////////////////////////////////////////////
void SolverAnimator::setTimeModifier(float t_modifier)
{
	m_timeModifer = t_modifier;

	for (MazeSolver* solver : m_solversRef)
	{
		if (solver->isAnimatingIn() || solver->isAnimatingOut())
		{
			solver->setTimeModifier(t_modifier, 20);
		}
	}
}

///////////////////////////////////////////////////////////////////////////
void SolverAnimator::animateIn(MazeSolver* t_solver)
{
	// If the move timer is up
	if (t_solver->getMoveTimer() == 0)
	{
		// If the solver has reached the maze vertically
		if (t_solver->getPos().y == 1)
		{
			if (t_solver->getPos().x == 0)
			{
				t_solver->reset(0);
				t_solver->setAnimatingIn(false);
				t_solver->setTimeModifier(m_timeModifer);
			}
			else
			{
				moveSolver(t_solver, Direction::East);
			}
		}
		else // If the solver has not reached the maze vertically
		{
			moveSolver(t_solver, Direction::North);
		}
	}
	else
	{
		t_solver->decrementMoveTimer();
		t_solver->animate();
	}
}

///////////////////////////////////////////////////////////////////////////
void SolverAnimator::animateOut(MazeSolver* t_solver)
{
	// If the move timer is up
	if (t_solver->getMoveTimer() == 0)
	{
		if (t_solver->getPos().x > -4)
		{
			moveSolver(t_solver, Direction::West);
		}
		else if (t_solver->getPos().y < 4)
		{
			moveSolver(t_solver, Direction::South);
		}
		else if (t_solver->getPos().x > -8)
		{
			moveSolver(t_solver, Direction::West);
		}
		else
		{
			t_solver->setAnimatingOut(false);
			t_solver->setActive(false);
		}
	}
	else
	{
		t_solver->decrementMoveTimer();
		t_solver->animate();
	}
}

///////////////////////////////////////////////////////////////////////////
void SolverAnimator::moveSolver(MazeSolver* t_solver, Direction t_direction)
{
	t_solver->resetMoveTimer();

	sf::Vector2i pos = t_solver->getPos();
	sf::Vector2i newPos = pos + Global::getDirectionVector(t_direction);

	t_solver->setPos(newPos.y, newPos.x);
	t_solver->setPreviousPos(pos.y, pos.x);

	t_solver->setMovementDirection(t_direction);
}

///////////////////////////////////////////////////////////////////////////