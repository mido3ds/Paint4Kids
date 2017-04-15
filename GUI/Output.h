#ifndef OUPTUT_H
#define OUPTUT_H

#include "Input.h"

class Output // The application manager should have a pointer to this class
{
public:
    Output();
    ~Output();

    window* CreateWind(int, int, int, int) const; // main window

    void CreateDrawToolBar() const; // creates Draw mode toolbar & menu
	void CreateFigItems() const;
	void CreateFigActions() const;
    void CreatePlayToolBar() const; // creates Play mode toolbar & menu

    void CreateStatusBar() const;
    void CreateColorBar() const;

    void CreateDrawArea() const;
    void CreatePlayArea() const;

    Input* CreateInput() const; // creates a pointer to the Input object

    void ClearStatusBar() const;
    void ClearDrawArea() const;
    void ClearToolBar() const;
	void ClearTToolBar() const;

    // -- Figures Drawing functions
    void Output::DrawRect(Point p1, Point p2, GfxInfo rect_gfx_info,
        bool selected) const;

    void DrawCircle(Point p1, Point p2, GfxInfo circ_gfx_info,
        bool selected) const; // Draw a circle

    void DrawLine(Point p1, Point p2, GfxInfo line_gfx_info,
        bool selected) const; // Draw a line

    void DrawTriangle(Point p1, Point p2, Point p3, GfxInfo trngl_gfx_info,
        bool selected) const; // Draw a  tringle

    void PrintMessage(string msg) const; // on Status bar

    color GetCrntDrawColor() const;
    color GetCrntFillColor() const;
    int GetCrntPenWidth() const;

    void SetCrntDrawColor(color given_clr);
    void SetCrntFillColor(color given_clr);
    void SetBkGrndColor(color given_clr);
    void SetCrntPenWidth(int new_width);

private:
    window* wind_p; // Pointer to the Graphics Window
};

#endif