#include "ApplicationManager.h"

//Constructor
ApplicationManager::ApplicationManager()
{
    //Create Input and output
    out_p = new Output;
    in_p = out_p->CreateInput();
}

//==================================================================================//
//								Actions Related Functions							//
//==================================================================================//
ActionType ApplicationManager::GetUserAction() const
{
    //Ask the input to get the action from the user.
    return in_p->GetUserAction();
}
////////////////////////////////////////////////////////////////////////////////////
// According to Action Type, return the corresponding action object
Action* ApplicationManager::DetectAction(ActionType act_type)
{
    switch (act_type) {
	case DRAW_FIG_ITM:
		out_p->CreateFigItems();
		return nullptr;
    case DRAW_RECT:
        return new AddRectAction(this);
    case DRAW_CIRC:
        return new AddCircAction(this);
    case DRAW_TRI:
        return new AddTrnglAction(this);
    case DRAW_LINE:
        return new AddLineAction(this);
    case EXIT:
        return new ExitAction(this);
    case TO_PLAY:
        return new SwitchPlayMode(this);
    case TO_DRAW:
        return new SwitchDrawMode(this);
    case SAVE:
        return new SaveAction(this);
    case LOAD:
        return new LoadAction(this);
    case UNDO:
        return new UndoAction(this);
    case REDO:
        return new RedoAction(this);
	case CHNG_FILL_CLR:
		return new ChFillColor(this);
	case CHNG_BK_CLR:
		return new ChBGColor(this);
	case CHNG_DRAW_CLR:
		return new ChBorder(this);
	case SEND_BACK:
		return new Down(this);
	case BRNG_FRNT:
		return new Up(this);
	case ROTATE:
		return new Rotate(this);
	case COLOR_BAR:
		out_p->ClearTToolBar();
		return nullptr;
	case CTR:
		out_p->CreateFigActions();
		return nullptr;
	case DEL:
		out_p->ClearTToolBar();
		return nullptr;
	case MOVE:
		out_p->ClearTToolBar();
		return nullptr;
	case RESIZE:
		out_p->ClearTToolBar();
		return nullptr;
	case COPY:
		out_p->ClearTToolBar();
		return nullptr;
	case PASTE:
		out_p->ClearTToolBar();
		return nullptr;
    case STATUS: //a click on the status bar ==> no action
        return nullptr;
    default:
        return nullptr;
    }
}
////////////////////////////////////////////////////////////////////////////////////
// gets action parameters and executes it
void ApplicationManager::ExecuteAction(ActionType act_type)
{
    Action* act_p = DetectAction(act_type);

    if (act_p != nullptr) {
        act_p->ReadActionParameters();
        act_p->Execute();

		// only add action if not (undo or redo or switchPlaymode or switchDrawMode)
		if (! (act_p->GetActType() == UNDO || act_p->GetActType() == REDO || act_p->GetActType() == TO_DRAW || act_p->GetActType() == TO_PLAY))
			undo_st.push(act_p);
    }
}
//==================================================================================//
//						Figures Management Functions								//
//==================================================================================//

//Add a figure to the list of figures
void ApplicationManager::AddFigure(CFigure* fig_p)
{
    figs.insert(fig_p);
}
////////////////////////////////////////////////////////////////////////////////////
CFigure* ApplicationManager::GetFigure(int x, int y) const
{
    //If a figure is found return a pointer to it.
    //if this point (x,y) does not belong to any figure return NULL

    ///Add your code here to search for a figure given a point x,y

    return NULL;
}
////////////////////////////////////////////////////////////////////////////////////
// According to given string, return the corresponding Figure object
CFigure* ApplicationManager::DetectFigure(string fig_name)
{
    if (fig_name == "RECTANGLE")
        return new CRectangle();
    if (fig_name == "CIRCLE")
        return new CCircle();
    if (fig_name == "TRIANGLE")
        return new CTrngl();
    if (fig_name == "LINE")
        return new CLine();
    throw - 1;
}
//==================================================================================//
//							Interface Management Functions							//
//==================================================================================//

//Draw all figures on the user interface
void ApplicationManager::UpdateInterface() const
{
	//out_p->ClearDrawArea();
    for (auto& fig : figs)
        fig->Draw(out_p); //Call Draw function (virtual member fn)
}
////////////////////////////////////////////////////////////////////////////////////
//Return a pointer to the input
Input* ApplicationManager::GetInput() const
{
    return in_p;
}
//Return a pointer to the output
Output* ApplicationManager::GetOutput() const
{
    return out_p;
}
////////////////////////////////////////////////////////////////////////////////////
// iterate through all figures
// call save for each one
void ApplicationManager::SaveAll(ofstream& out_file)
{
    // TODO
    out_file << UI.DrawColor.ucRed << ' '
             << UI.DrawColor.ucGreen << ' '
             << UI.DrawColor.ucBlue << ' '

             << UI.FillColor.ucRed << ' '
             << UI.FillColor.ucGreen << ' '
             << UI.FillColor.ucBlue << ' '

             << UI.BkGrndColor.ucRed << ' '
             << UI.BkGrndColor.ucGreen << ' '
             << UI.BkGrndColor.ucBlue << '\n';
    out_file << figs.size() << '\n';

    for (auto& fig : figs)
        fig->Save(out_file);
}
// iterate through lines and make the apropriate figure
// call load for the figure
void ApplicationManager::LoadAll(ifstream& in_file)
{
    // TODO
    int size = 0;
    string fig_name;
    CFigure* fig = nullptr;

    in_file >> UI.DrawColor.ucRed
        >> UI.DrawColor.ucGreen
        >> UI.DrawColor.ucBlue

        >> UI.FillColor.ucRed
        >> UI.FillColor.ucGreen
        >> UI.FillColor.ucBlue

        >> UI.BkGrndColor.ucRed
        >> UI.BkGrndColor.ucGreen
        >> UI.BkGrndColor.ucBlue;

    in_file >> size;

    for (int i = 0; i < size; i++) {
        in_file >> fig_name;
        fig = DetectFigure(fig_name);
        fig->Load(in_file);

        figs.insert(fig);
    }
}
////////////////////////////////////////////////////////////////////////////////////
void ApplicationManager::Undo()
{
	if (undo_st.empty())
		return;

    Action* to_undo = undo_st.top();

    to_undo->Undo();
    redo_st.push(to_undo);
    undo_st.pop();
}

void ApplicationManager::Redo()
{
	if (redo_st.empty())
		return;

    Action* to_redo = redo_st.top();

    to_redo->Execute();
    undo_st.push(to_redo);
    redo_st.pop();
}
////////////////////////////////////////////////////////////////////////////////////

unsigned int ApplicationManager::GenerateNextId()
{
    return next_id++;
}

void ApplicationManager::DeleteFigure(unsigned int id)
{
    auto itr = GetFigureIter(id);
    if (itr != figs.end())
    {
        delete (*itr);
        figs.erase(itr);
    }
    else
    {
        // cant delete figure not found
        throw -1;
    }
}

CFigure* ApplicationManager::GetFigure(unsigned int id) const
{
    for (auto& fig : figs)
        if (fig->GetId() == id)
            return fig;
    return nullptr;
}

multiset<CFigure*, CmpFigures>::iterator ApplicationManager::GetFigureIter(unsigned int id) const
{
    for (auto itr = figs.begin(); itr != figs.end(); itr++)
        if ((*itr)->GetId() == id)
            return itr;

    return figs.end();
}

bool ApplicationManager::ChangeSelectedFillColor(color c)
{
	bool flag = false;
	for (auto itr = figs.begin(); itr != figs.end(); itr++) {
		if ((*itr)->IsSelected())
		{
			(*itr)->ChngFillClr(c);
			flag = true;
		}
	}
	return flag;
}

bool ApplicationManager::ChangeSelectedBorder(int W, color C)
{
	bool flag = false;
	for (auto itr = figs.begin(); itr != figs.end(); itr++) {
		if ((*itr)->IsSelected())
		{
			(*itr)->ChngDrawClr(C);
			(*itr)->ChngBorderWidth(W);
			(*itr)->SetSelected(false);
			flag = true;
		}
	}
	return flag;
}

void ApplicationManager::SendSelecteDown()
{
	int x;
	for (auto itr = figs.begin(); itr != figs.end(); itr++) {
		if ((*itr)->IsSelected())
		{
			x = (*itr)->z_index;
			for (auto itr2 = figs.begin(); itr2 != figs.end(); itr2++) {
				if (x >= (*itr2)->z_index && itr != itr2)
				{
					x = (*itr2)->z_index - 1;
				}
			}
			(*itr)->ChngZindex(x);
			(*itr)->SetSelected(false);
			multiset <CFigure *, CmpFigures> list(figs.begin(), figs.end());
			figs = list;
			itr = figs.begin();
		}
	}
}

void ApplicationManager::BringSelectedUp()
{
	int x;
	for (auto itr = figs.begin(); itr != figs.end(); itr++) {
		if ((*itr)->IsSelected())
		{
			x = (*itr)->z_index;
			for (auto itr2 = figs.begin(); itr2 != figs.end(); itr2++) {
				if (x <= (*itr2)->z_index && itr != itr2)
				{
					x = (*itr2)->z_index + 1;
				}
			}
			(*itr)->ChngZindex(x);
			(*itr)->SetSelected(false);
			multiset <CFigure *, CmpFigures> list(figs.begin(), figs.end());
			figs = list;
			itr = figs.begin();
		}
	}
}

void ApplicationManager::RotateSelected(int deg)
{
	for (auto itr = figs.begin(); itr != figs.end(); itr++) {
		if ((*itr)->IsSelected())
		{
			(*itr)->Rotate(deg);
			if ((*itr)->IsRotate())
			{
				(*itr)->Rotated(false);
			}
			else {
				out_p->PrintMessage("This Figure Is Out Of Range If Rotated");
			}
		}
	}
}

////////////////////////////////////////////////////////////////////////////////////
//Destructor
ApplicationManager::~ApplicationManager()
{
    for (auto& fig : figs)
        delete fig;
    delete in_p;
    delete out_p;
}
