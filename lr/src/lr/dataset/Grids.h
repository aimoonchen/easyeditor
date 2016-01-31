#ifndef _LR_GRIDS_H_
#define _LR_GRIDS_H_



namespace lr
{

class Grids
{
public:
	Grids() {}

	void Draw() const;
	void Build(int width, int height);

	std::vector<int> IntersectPolygon(const std::vector<ee::Vector>& poly) const;
	std::vector<int> IntersectPolyline(const std::vector<ee::Vector>& path) const;

	void GetGridSize(int& col, int& row) {
		col = m_col;
		row = m_row;
	}

	// for debug
	void SetDebbugDrawGrids(const std::vector<int>& grids) {
		copy(grids.begin(), grids.end(), back_inserter(m_debug_draw_grids));
	}

private:
	static ee::Vector TransToBirdView(float x, float y);
	static ee::Vector TransToFlatView(float x, float y);

	void GetGridRegion(const std::vector<ee::Vector>& area, int& xmin, int& xmax, int& ymin, int& ymax) const;

	static void TransVerticesToFlat(const std::vector<ee::Vector>& src, std::vector<ee::Vector>& dst);

public:
	static const float EDGE;

private:
	static const float PROJ_TRANS;
	static const float BIRD_HW, BIRD_HH;

	struct Grid
	{
		Grid(float left, float top);

		// top left bottom right
		std::vector<ee::Vector> m_bird_bound;
		ee::Rect m_flat_bound;
	}; // Grid

private:
	int m_width, m_height;
	int m_col, m_row;
	std::vector<Grid> m_grids;

	std::vector<ee::Vector> m_flat_bound;

	std::vector<int> m_debug_draw_grids;

}; // Grids

}

#endif // _LR_GRIDS_H_