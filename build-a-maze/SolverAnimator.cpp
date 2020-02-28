#include "SolverAnimator.h"

SolverAnimator::SolverAnimator(sf::RenderWindow& t_window, std::vector<MazeSolver*>& t_solvers) :
	m_windowRef{ t_window },
	m_solversRef{ t_solvers },
	m_animating{ false }
{
}

void SolverAnimator::update()
{
	// if the solvers are animating
	if (m_animating)
	{
		// Loop all solvers
		for (MazeSolver* solver : m_solversRef)
		{
			// If the individual solver is still animating
			if (solver->isAnimatingOutside())
			{
				// If the move timer is up
				if (solver->getMoveTimer() == 0)
				{
					// If the solver has reached the maze vertically
					if (solver->getPos().y == 1)
					{
						if (solver->getPos().x == 0)
						{
							solver->reset(0);
							solver->setAnimatingOutside(false);
						}
						else
						{
							solver->setMoveTimer(20);

							sf::Vector2i pos = solver->getPos();

							solver->setPos(pos.y, pos.x + 1);

							solver->setPreviousPos(pos.y, pos.x);

							solver->setMovementDirection(Direction::East);
						}
					}
					else // If the solver has not reached the maze vertically
					{
						solver->setMoveTimer(20);

						sf::Vector2i pos = solver->getPos();

						solver->setPos(pos.y - 1, pos.x);

						solver->setPreviousPos(pos.y, pos.x);
					}
				}
				else
				{
					solver->setMoveTimer(solver->getMoveTimer() - 1);
					solver->animate();
				}
			}
		}
	}
}

void SolverAnimator::draw()
{
	for (MazeSolver * solver : m_solversRef)
	{
		solver->draw(m_windowRef);
	}
}

void SolverAnimator::startAnimating()
{
	m_animating = true;

	int i = 0;
	for (MazeSolver* solver : m_solversRef)
	{
		solver->setMoveTimer(20);
		solver->setPos(1, -8 - (i * 3));
		solver->setAnimatingOutside(true);
		solver->setMovementDirection(Direction::East);
		i++;
	}
}

const bool SolverAnimator::isAnimating() const
{
	return m_animating;
}
