#include "helloworld.h"
#include <QtPlugin>
#include <QMessageBox>


using namespace MOBase;


HelloWorld::HelloWorld()
{
  // constructor. Please note that this is called before MO is started up completely so
  // you can not do anything that would interface with the main application here.
}

bool HelloWorld::init(IOrganizer *organizer)
{
  // initialize the plugin. This happens after the main application is largely initialized, except
  // for the virtual directory structure. That is loaded in parallel and may take a moment to be complete.

  // usually you will want to save the "organizer" reference to a member variable because this is your
  // gateway to most functions of MO

  // return true if the plugin started correctly. If you return false this will be considered a "problem", but
  // the main application does not do anything about it except for printing a warning.

  // if you want to give more detailed information to the user, print your own warning or - much more convenient for the user -
  // implement the IPluginDiagnose interface.
  return true;
}

QString HelloWorld::name() const
{
  // the "internal" name of your plugin. This is the name under which it will show up in the settings dialog
  return tr("Hello World");
}

QString HelloWorld::author() const
{
  // your name
  return "Tannin";
}

QString HelloWorld::description() const
{
  // a description of your plugin. This should be short and descriptive
  return tr("Most basic plugin you can write in C++");
}

VersionInfo HelloWorld::version() const
{
  // version of the plugin. Please ensure to update this with every release
  return VersionInfo(1, 0, 0, VersionInfo::RELEASE_FINAL);
}

bool HelloWorld::isActive() const
{
  // return true if the plugin is active. This allows you to disable the plugin in cases that don't constitute
  // an error.
  // For example if you write a plugin that works only with Skyrim you can return false here if the active game
  // is not skyrim
  return false;
}

QList<PluginSetting> HelloWorld::settings() const
{
  // return a list of user-configurable settings for this plugins to be exposed through the settings dialog of MO.
  // you can access the values for these settings through IOrganizer::pluginSetting
  return QList<PluginSetting>();
}

QString HelloWorld::displayName() const
{
  // the name of this tool as displayed in the toolbar
  return tr("Hello World");
}

QString HelloWorld::tooltip() const
{
  // tooltip, also displayed in the toolbar
  return tr("Gives a friendly greeting");
}

QIcon HelloWorld::icon() const
{
  // icon in the toolbar. You should manage this icon (and all other graphics/assets you need) in a resource file
  // that gets included in the dll. Please check the other plugins to see how this works.
  return QIcon();
}

void HelloWorld::display() const
{
  // display gets called when the user activates your plugin. This is basically the main entry point of your tool plugin
  QMessageBox::information(parentWidget(), tr("Hello"), tr("Hello World"));
}

#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(helloWorld, HelloWorld)
#endif
