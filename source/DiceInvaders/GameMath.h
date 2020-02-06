
// Namespace to collect all the math classes
namespace GameMath
{
	class RectangleOperation
	{
	public:
		struct rect
		{
			int x;
			int y;
			int width;
			int height;
		};
	
		// Check if two rectangles overlap
		static bool rectOverlap(rect A, rect B)
		{
			bool xOverlap = valueInRange(A.x, B.x, B.x + B.width) ||
							valueInRange(B.x, A.x, A.x + A.width);

			bool yOverlap = valueInRange(A.y, B.y, B.y + B.height) ||
							valueInRange(B.y, A.y, A.y + B.height);

			return xOverlap && yOverlap;
		}

	private:
		static bool valueInRange(int value, int min, int max)
		{ return (value >= min) && (value <= max); }

	};
}