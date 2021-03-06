#ifndef __CH_FILL_COLOR__
#define __CH_FILL_COLOR__

#include "../ApplicationManager.h"
#include "../CMUgraphicsLib/colors.h"
#include "../Figures/CRectangle.h"
#include "Action.h"

#include "../GUI/Input.h"
#include "../GUI/Output.h"

class ChFillColorAction : public Action {
public:
    ChFillColorAction(ApplicationManager* app_p);

    virtual void ReadActionParameters();

    virtual void Execute(bool redo = false);

    virtual void Undo();

private:
    color fill_clr = UI.LastFillColor,
          last_fill_clr = fill_clr;

	bool is_filled = UI.IsFilled;

	deque<int> IDs;
};

#endif // !_CH_FILL_COLOR_