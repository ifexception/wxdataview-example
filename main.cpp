#include <wx/wx.h>
#include <wx/dataview.h>

class MyTreeModelNode;
WX_DEFINE_ARRAY_PTR(MyTreeModelNode*, MyTreeModelNodePtrArray);

class MyTreeModelNode final
{
public:
    MyTreeModelNode(MyTreeModelNode* parent,
        int id,
        const wxString& column1,
        const wxString& column2,
        const wxString& column3,
        const wxString& column4);
    MyTreeModelNode(MyTreeModelNode* parent, const wxString& branch);
    ~MyTreeModelNode();

    bool IsContainer() const;
    MyTreeModelNode* GetParent();
    MyTreeModelNodePtrArray& GetChildren();
    MyTreeModelNode* GetNthChild(unsigned int n);

    void Insert(MyTreeModelNode* child, unsigned int n);
    void Append(MyTreeModelNode* child);
    const unsigned int GetChildCount() const;

    int GetId() const;
    wxString GetColumn1() const;
    wxString GetColumn2() const;
    wxString GetColumn3() const;
    wxString GetColumn4() const;

    void SetId(int value);
    void SetColumn1(const wxString& value);
    void SetColumn2(const wxString& value);
    void SetColumn3(const wxString& value);
    void SetColumn4(const wxString& value);

private:
    MyTreeModelNode* pParent;
    MyTreeModelNodePtrArray mChildren;

    int mId;
    wxString mColumn1;
    wxString mColumn2;
    wxString mColumn3;
    wxString mColumn4;
    bool bContainer;
};

MyTreeModelNode::MyTreeModelNode(MyTreeModelNode* parent,
    int id,
    const wxString& column1,
    const wxString& column2,
    const wxString& column3,
    const wxString& column4)
    : pParent(parent)
    , mId(id)
    , mColumn1(column1)
    , mColumn2(column2)
    , mColumn3(column3)
    , mColumn4(column4)
    , bContainer(false)
{
}

MyTreeModelNode::MyTreeModelNode(MyTreeModelNode* parent, const wxString& branch)
    : pParent(parent)
    , mColumn1(branch)
    , bContainer(true)
{
}

MyTreeModelNode::~MyTreeModelNode()
{
    std::size_t count = mChildren.GetCount();
    for (std::size_t i = 0; i < count; i++) {
        MyTreeModelNode* child = mChildren[i];
        delete child;
    }
}

bool MyTreeModelNode::IsContainer() const
{
    return bContainer;
}

MyTreeModelNode* MyTreeModelNode::GetParent()
{
    return pParent;
}

MyTreeModelNodePtrArray& MyTreeModelNode::GetChildren()
{
    return mChildren;
}

MyTreeModelNode* MyTreeModelNode::GetNthChild(unsigned int n)
{
    return mChildren.Item(n);
}

void MyTreeModelNode::Insert(MyTreeModelNode* child, unsigned int n)
{
    mChildren.Insert(child, n);
}

void MyTreeModelNode::Append(MyTreeModelNode* child)
{
    mChildren.Add(child);
}

const unsigned int MyTreeModelNode::GetChildCount() const
{
    return mChildren.Count();
}

int MyTreeModelNode::GetId() const
{
    return mId;
}

wxString MyTreeModelNode::GetColumn1() const
{
    return mColumn1;
}

wxString MyTreeModelNode::GetColumn2() const
{
    return mColumn2;
}

wxString MyTreeModelNode::GetColumn3() const
{
    return mColumn3;
}

wxString MyTreeModelNode::GetColumn4() const
{
    return mColumn4;
}

void MyTreeModelNode::SetId(int value)
{
    mId = value;
}

void MyTreeModelNode::SetColumn1(const wxString& value)
{
    mColumn1 = value;
}

void MyTreeModelNode::SetColumn2(const wxString& value)
{
    mColumn2 = value;
}

void MyTreeModelNode::SetColumn3(const wxString& value)
{
    mColumn3 = value;
}

void MyTreeModelNode::SetColumn4(const wxString& value)
{
    mColumn4 = value;
}

class MyTreeModel : public wxDataViewModel
{
public:
    enum { Col_Column1 = 0, Col_Column2, Col_Column3, Col_Column4, Col_Id, Col_Max };

    MyTreeModel();
    ~MyTreeModel();

    unsigned int GetColumnCount() const override;
    wxString GetColumnType(unsigned int col) const override;
    void GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const override;
    bool SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col) override;
    bool IsEnabled(const wxDataViewItem& item, unsigned int col) const override;
    wxDataViewItem GetParent(const wxDataViewItem& item) const override;
    bool IsContainer(const wxDataViewItem& item) const override;
    unsigned int GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const override;

private:
    MyTreeModelNode* pRoot;
    MyTreeModelNode* pDataSet1;
    MyTreeModelNode* pDataSet2;
};


MyTreeModel::MyTreeModel()
{
    pRoot = new MyTreeModelNode(nullptr, wxT("Data Overview"));

    pDataSet1 = new MyTreeModelNode(pRoot, wxT("Data Set #1"));

    auto nodeDataSet1 = new MyTreeModelNode(pRoot, 455, "A data set", "describes", "useful", "information to a end user (hopefully)");
    pDataSet1->Append(nodeDataSet1);

    pRoot->Append(pDataSet1);

    pDataSet2 = new MyTreeModelNode(pRoot, wxT("Data Set #2"));
    pRoot->Append(pDataSet2);
}

MyTreeModel::~MyTreeModel()
{
    delete pRoot;
}

unsigned int MyTreeModel::GetColumnCount() const
{
    return Col_Max;
}

wxString MyTreeModel::GetColumnType(unsigned int col) const
{
    if (col == Col_Id) {
        return "long";
    }
    else {
        return "string";
    }
}

void MyTreeModel::GetValue(wxVariant& variant, const wxDataViewItem& item, unsigned int col) const
{
    wxASSERT(item.IsOk());

    MyTreeModelNode* node = (MyTreeModelNode*)item.GetID();
    switch (col) {
    case Col_Column1:
        variant = node->GetColumn1();
        break;
    case Col_Column2:
        variant = node->GetColumn2();
        break;
    case Col_Column3:
        variant = node->GetColumn3();
        break;
    case Col_Column4:
        variant = node->GetColumn4();
        break;
    case Col_Id:
        variant = (long)node->GetId();
        break;
    case Col_Max:
    default:
        wxLogError("MyTreeModel::GetValue: wrong column %d", col);
        break;
    }
}

bool MyTreeModel::SetValue(const wxVariant& variant, const wxDataViewItem& item, unsigned int col)
{
    wxASSERT(item.IsOk());

    MyTreeModelNode* node = (MyTreeModelNode*)item.GetID();
    switch (col) {
    case Col_Column1:
        node->SetColumn1(variant.GetString());
        break;
    case Col_Column2:
        node->SetColumn2(variant.GetString());
        break;
    case Col_Column3:
        node->SetColumn3(variant.GetString());
        break;
    case Col_Column4:
        node->SetColumn4(variant.GetString());
        break;
    case Col_Id:
        node->SetId(variant.GetInteger());
        break;
    case Col_Max:
    default:
        wxLogError("MyTreeModel::SetValue: wrong column %d", col);
        break;
    }

    return false;
}

bool MyTreeModel::IsEnabled(const wxDataViewItem& item, unsigned int col) const
{
    return true;
}

wxDataViewItem MyTreeModel::GetParent(const wxDataViewItem& item) const
{
    if (!item.IsOk()) {
        return wxDataViewItem(0);
    }

    MyTreeModelNode* node = (MyTreeModelNode*)item.GetID();

    if (node == pRoot) {
        return wxDataViewItem(0);
    }

    return wxDataViewItem((void*)node->GetParent());
}

bool MyTreeModel::IsContainer(const wxDataViewItem& item) const
{
    if (!item.IsOk()) {
        return true;
    }

    MyTreeModelNode* node = (MyTreeModelNode*)item.GetID();
    return node->IsContainer();
}

unsigned int MyTreeModel::GetChildren(const wxDataViewItem& parent, wxDataViewItemArray& array) const
{
    MyTreeModelNode* node = (MyTreeModelNode*)parent.GetID();
    if (!node) {
        array.Add(wxDataViewItem((void*)pRoot));
        return 1;
    }

    if (node->GetChildCount() == 0) {
        return 0;
    }

    unsigned int count = node->GetChildren().GetCount();
    for (unsigned int pos = 0; pos < count; pos++) {
        MyTreeModelNode* child = node->GetChildren().Item(pos);
        array.Add(wxDataViewItem((void*)child));
    }

    return count;
}

class MainFrame final : public wxFrame
{
public:
    MainFrame();

    virtual ~MainFrame() = default;

private:
    void CreateControls();
    wxObjectDataPtr<MyTreeModel> pMyTreeModel;
    wxDataViewCtrl* pDataViewCtrl;

    enum { IDC_DATAVIEW = wxID_HIGHEST + 1 };

};

MainFrame::MainFrame()
    : wxFrame(nullptr, wxID_ANY, wxT("wxWidgets Dataview Example"))
{
    CreateControls();
}

void MainFrame::CreateControls()
{
    auto mainPanel = new wxPanel(this, wxID_ANY);

    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    mainPanel->SetSizer(mainSizer);

    /* Data View Ctrl */
    long style = wxDV_SINGLE | wxDV_ROW_LINES | wxDV_HORIZ_RULES | wxDV_VERT_RULES;
    pDataViewCtrl = new wxDataViewCtrl(mainPanel, IDC_DATAVIEW, wxDefaultPosition, wxDefaultSize, style);
    mainSizer->Add(pDataViewCtrl, 1, wxEXPAND | wxALL, 5);

    /* Data View Model */
    pMyTreeModel = new MyTreeModel();
    pDataViewCtrl->AssociateModel(pMyTreeModel.get());

    /* Data View Columns */
    auto column1TextRenderer = new wxDataViewTextRenderer("string", wxDATAVIEW_CELL_INERT);
    auto column2TextRenderer = new wxDataViewTextRenderer("string", wxDATAVIEW_CELL_INERT);
    auto column3TextRenderer = new wxDataViewTextRenderer("string", wxDATAVIEW_CELL_INERT);
    auto column4TextRenderer = new wxDataViewTextRenderer("string", wxDATAVIEW_CELL_INERT);
    auto idColumnRenderer = new wxDataViewTextRenderer("long", wxDATAVIEW_CELL_INERT);

    auto column1 = new wxDataViewColumn(wxT("Column 1"),
        column1TextRenderer,
        MyTreeModel::Col_Column1,
        80,
        wxALIGN_LEFT,
        wxDATAVIEW_COL_RESIZABLE);
    column1->SetWidth(wxCOL_WIDTH_AUTOSIZE);
    pDataViewCtrl->AppendColumn(column1);

    auto column2 =
        new wxDataViewColumn(wxT("Column 2"), column2TextRenderer, MyTreeModel::Col_Column2);
    column2->SetWidth(wxCOL_WIDTH_AUTOSIZE);
    column2->SetResizeable(false);
    pDataViewCtrl->AppendColumn(column2);

    auto column3 = new wxDataViewColumn(wxT("Column 3"),
        column3TextRenderer,
        MyTreeModel::Col_Column3,
        80,
        wxALIGN_CENTER,
        wxDATAVIEW_COL_RESIZABLE);
    column3->SetWidth(wxCOL_WIDTH_AUTOSIZE);
    pDataViewCtrl->AppendColumn(column3);

    auto column4 = new wxDataViewColumn(wxT("Column 4"),
        column4TextRenderer,
        MyTreeModel::Col_Column4,
        80,
        wxALIGN_LEFT,
        wxDATAVIEW_COL_RESIZABLE);
    pDataViewCtrl->AppendColumn(column4);


    /* ID Column */
    auto idColumn = new wxDataViewColumn(
        wxT("ID"), idColumnRenderer, MyTreeModel::Col_Id, 32, wxALIGN_CENTER, wxDATAVIEW_COL_HIDDEN);
    pDataViewCtrl->AppendColumn(idColumn);
}

class Application : public wxApp
{
public:
    Application();
    virtual ~Application() = default;

    bool OnInit() override;
};

Application::Application()
{
}

bool Application::OnInit()
{
    auto frame = new MainFrame();
    frame->Show(true);
    SetTopWindow(frame);

    return true;
}

wxIMPLEMENT_APP(Application);