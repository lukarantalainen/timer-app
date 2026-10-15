#include <QMenuBar>
class QWidget;
class Keyboard;

class MenuBar : public QMenuBar {
  public:
  MenuBar(QWidget* parent = nullptr);
  MenuBar(Keyboard* keyboard, QWidget* parent = nullptr);

  private:
  Keyboard* keyboard = nullptr;
};
