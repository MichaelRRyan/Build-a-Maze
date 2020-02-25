#ifndef SPEECH_BUBBLE_SHAPE_H
#define SPEECH_BUBBLE_SHAPE_H

#include <SFML/Graphics/Shape.hpp>

namespace sf
{
	/// <summary>
	/// @brief a class to represent a rounded rectangle shape
	/// </summary>
	class RoundedRectangleShape : public sf::Shape
	{
	public:

		/// <summary>
		/// @brief setup the shape
		/// </summary>
		/// <param name="t_size">size</param>
		/// <param name="t_cornerRadius">corner radius</param>
		/// <param name="t_cornerPointCount">number of points in a corner</param>
		explicit RoundedRectangleShape(sf::Vector2f t_size = { 0.0f, 0.0f }, float t_cornerRadius = 0.0f, std::size_t t_cornerPointCount = 15);

		/// <summary>
		/// @brief sets the size of the shape
		/// </summary>
		/// <param name="t_size">size</param>
		void setSize(const sf::Vector2f t_size);

		/// <summary>
		/// @brief returns the size of the shape
		/// </summary>
		/// <returns>shape size</returns>
		sf::Vector2f getSize() const;

		/// <summary>
		/// @brief sets the radius of the corner
		/// </summary>
		/// <param name="t_radius">corner radius</param>
		void setCornerRadius(const float t_radius);

		/// <summary>
		/// @brief returns the radius of the corner
		/// </summary>
		/// <returns>corner radius</returns>
		float getCornerRadius() const;

		/// <summary>
		/// @brief sets the number of points in each corner
		/// </summary>
		/// <param name="t_cornerPointCount">point count</param>
		void setCornerPointCount(const std::size_t t_cornerPointCount);

		/// <summary>
		/// @brief returns the number of points in the entire shape
		/// </summary>
		/// <returns></returns>
		virtual std::size_t getPointCount() const;

		/// <summary>
		/// @brief get the pixel coordinates of a point at an index
		/// </summary>
		/// <param name="t_index"></param>
		/// <returns>pixel coordinate</returns>
		virtual sf::Vector2f getPoint(std::size_t t_index) const;

	private:
		
		// Private data members
		std::size_t m_cornerPointCount; // Number of points in a corner
		float m_cornerRadius; // Radius of each corner
		sf::Vector2f m_size; // Size of the shape
	};
}


#endif // !SPEECH_BUBBLE_SHAPE_H