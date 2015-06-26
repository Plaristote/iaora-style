/*
 * sk_config.h
 *
 */

#ifndef SKULPTURE_CONFIG_H
#define SKULPTURE_CONFIG_H 1


/*-----------------------------------------------------------------------*/

#include <QtGui/QWidget>
#include "ui_sk_dialog.h"
class QSettings;
class QMdiSubWindow;

class SkulptureStyleConfig : public QWidget, private Ui::SkulptureStyleDialog
{
	Q_OBJECT

	public:
		explicit SkulptureStyleConfig(QWidget* parent = 0);
		virtual ~SkulptureStyleConfig();

		virtual QSize sizeHint() const;

	Q_SIGNALS:
		void changed(bool);

	public Q_SLOTS:
		void save();
		void defaults();

	protected:
		void load();
		void apply();
		void saveSettings(QSettings &settings);
		void loadSettings(QSettings &settings);

	protected Q_SLOTS:
		void updateChanged();
		void updatePreview();

	private Q_SLOTS:
		void subWindowActivated(QMdiSubWindow *window);

	private:
		void init();

	private:
		bool savedAnimateProgressBars;
		bool savedAllowScrollBarSliderToCoverArrows;
		bool savedHideShortcutUnderlines;
};


/*-----------------------------------------------------------------------*/

enum ThemeCategory {
	Colors,
	Fonts,		// text attributes
	Frames,		// style of layout
	Layout,		// sizing, spacing
	Decorations,	// grips etc.
	Gradients,
	Accessibility,
	Icons,
	Effects,		// animations, fading, light
	Cursors,
	Sounds,
};


/*-----------------------------------------------------------------------*/

class KThemePreview : public QWidget
{
	Q_OBJECT

	public:
		enum ThemeRole { General, Fixed, Small, Toolbar, MainToolbar, Menu, Window, TaskBar, Desktop };

	public:
		explicit KThemePreview(QWidget *parent = 0);
		virtual ~KThemePreview();

	public:
		using QWidget::setFont;
		void setFont(ThemeRole role, const QFont &font);
		// TODO font settings, such as antialiasing and hinting

		void setIconSize(ThemeRole role, const QSize &size);
		void setIconTheme(const QString &iconTheme);
		// TODO icon effects, icon animation, double size

		void setWindowDecoration(const QString &windowDecoration);
		// TODO window buttons, window theme settings

		// setStyle(QStyle *);
		void setSkulpturePrivateSettings(QSettings &s);
		// TODO style effects, toolbar options

	protected:
		void addWindow(QWidget *window);
		void addPage(QWidget *widget);

		void removeWindow(QWidget *window);
		void removePage(QWidget *page);

	private:
		class Private;
		Private * const d;
};


/*-----------------------------------------------------------------------*/

#include <QtGui/QStyle>

struct SkMethodData
{
	int version;
};


struct SkMethodDataSetSettingsFileName : public SkMethodData
{
	// in version 1
	QString fileName;
};


class SkulptureStyle : public QStyle
{
	Q_OBJECT

	public:
		// internal, reserved for future use
		enum SkulpturePrivateMethod {
			SPM_SupportedMethods = 0,
			SPM_SetSettingsFileName = 1,
		};

	public Q_SLOTS:
		int skulpturePrivateMethod(SkulpturePrivateMethod /*id*/, void */*data*/ = 0) {
			return 0;
		}
};


/*-----------------------------------------------------------------------*/

#endif


