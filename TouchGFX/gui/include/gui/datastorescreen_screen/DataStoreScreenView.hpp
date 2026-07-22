#ifndef DATASTORESCREENVIEW_HPP
#define DATASTORESCREENVIEW_HPP

#include <gui_generated/datastorescreen_screen/DataStoreScreenViewBase.hpp>
#include <gui/datastorescreen_screen/DataStoreScreenPresenter.hpp>

class DataStoreScreenView : public DataStoreScreenViewBase
{
public:
    DataStoreScreenView();
    virtual ~DataStoreScreenView() {}
    virtual void setupScreen();
    virtual void tearDownScreen();
protected:
};

#endif // DATASTORESCREENVIEW_HPP
