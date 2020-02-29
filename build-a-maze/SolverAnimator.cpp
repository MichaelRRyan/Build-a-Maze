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
		solver->setPos(GAME_EXIT.y, GAME_EXIT.x - (i * 3));
		solver->setAnimatingIn(true);
		solver->setMovementDirection(Direction::East);
		solver->animate();
		i++;
	}
}

///////////////////////////////////////////////////////////////////////////
void SolverAnimator::animateIn(MazeSolver* t_solver, float t_modifier)
{
	m_animating = true;

	t_solver->setTimeModifier(t_modifier, 20);
	t_solver->setPos(GAME_EXIT.y, GAME_EXIT.x);
	t_solver->setAnimatingIn(true);
	t_solver->setMovementDirection(Direction::East);
	t_solver->animate();
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
		if (t_solver->getPos().x < -3)
		{
			moveSolver(t_solver, Direction::East);
		}
		else if (t_solver->getPos().y > 1)
		{
			moveSolver(t_solver, Direction::North);
		}
		else if (t_solver->getPos().x < 0)
		{
			moveSolver(t_solver, Direction::East);
		}
		else
		{
			t_solver->reset(0);
			t_solver->setAnimatingIn(false);
			t_solver->setTimeModifier(m_timeModifer);
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
		else if (t_solver->getPos().y < GAME_EXIT.y)
		{
			moveSolver(t_solver, Direction::South);
		}
		else if (t_solver->getPos().x > GAME_EXIT.x)
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