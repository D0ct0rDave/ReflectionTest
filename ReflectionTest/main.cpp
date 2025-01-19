#include "reflection.hpp"
using namespace agm::reflection;
using namespace std;


class Widget {
public:
    CLASS(Widget, NullClass);

    PROPERTY(int, x);
    PROPERTY(int, y);
    PROPERTY(int, width);
    PROPERTY(int, height);
    PROPERTY(bool, visible);
    PROPERTY(bool, enabled);

    STATIC_FIELD(public, const int, version);

    METHOD(public, bool, add, (Widget *child));

    STATIC_METHOD(public, const char *, get_version_string, ());
private:
    FIELD(private, int, m_x);
    FIELD(private, int, m_y);
    FIELD(private, int, m_width);
    FIELD(private, int, m_height);
    FIELD(private, bool, m_visible);
    FIELD(private, bool, m_enabled);

    int get_x() const {
        return m_x;
    }

    void set_x(int v) {
        m_x = v;
    }

    int get_y() const {
        return m_y;
    }

    void set_y(int v) {
        m_y = v;
    }

    int get_width() const {
        return m_width;
    }

    void set_width(int v) {
        m_width = v;
    }

    int get_height() const {
        return m_height;
    }

    void set_height(int v) {
        m_height = v;
    }

    bool get_visible() const {
        return m_visible;
    }

    void set_visible(bool v) {
        m_visible = v;
    }

    bool get_enabled() const {
        return m_enabled;
    }

    void set_enabled(bool v) {
        m_enabled = v;
    }

};


const int Widget::version = 1;


const char *Widget::get_version_string()
{
    return "1.0";
}


bool Widget::add(Widget *child)
{
    cout << "Added widget of class " << child->getClass().getName() << endl;
    if (child == 0) return false;
    return true;
}


class Label : public Widget {
public:
    CLASS(Label, Widget);
protected:
private:
};


int main1()
{
    Widget wgt;

    wgt.x = 5;
    int x = wgt.x;

    wgt.visible = true;
    bool v = wgt.visible;

    cout << "wgt.x=" << x << endl;
    cout << "wgt.visible=" << v << endl;

    getchar();
    return 0;
}


int main2()
{
    Widget wgt;

    wgt.x = 5;
    wgt.x++;
    int x = wgt.x;
    bool is_6 = wgt.x == 6;

    cout << "wgt.x=" << x << endl;
    cout << "wgt.x == 6 ? " << is_6 << endl;

    getchar();
    return 0;
}


int main3()
{
    Widget wgt;

    for(Class::PropertyList::const_iterator it = wgt.getClass().getProperties().begin();
        it != wgt.getClass().getProperties().end();
        ++it)
    {
        const Property &prop = *it;
        cout << prop.getType() << " " << prop.getName() << endl;
    }

    getchar();
    return 0;
}


int main4()
{
    Widget wgt;
    
    const Property &prop = wgt.getClass().getProperty("width");

    cout << prop.getType() << " " << prop.getName() << endl;

    prop.set(&wgt, 5);
    int w;
    prop.get(w, &wgt);

    cout << prop.getName() << " = " << w << endl;

    getchar();
    return 0;
}


int main()
{
    Widget wgt;
    Label lbl;

    bool ok;
    wgt.getClass().getMethod("add").invoke(ok, &wgt, (Widget *)&lbl);
    
    getchar();
    return 0;
}
