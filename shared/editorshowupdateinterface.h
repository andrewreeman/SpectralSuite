#ifndef EDITORSHOWUPDATEINTERFACE_H
#define EDITORSHOWUPDATEINTERFACE_H


class EditorShowUpdateInterface
{
    public:
        EditorShowUpdateInterface();
        virtual ~EditorShowUpdateInterface();
        virtual void showUpdate() = 0;
        virtual void closeUpdate() = 0;
    protected:
    private:
};

#endif // EDITORSHOWUPDATEINTERFACE_H
