#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlDatabase>
#include <QListWidgetItem>
#include <QTranslator>
#include <QVector>
#include <QLabel>
#include "weapon.h"

class weapon;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
	Q_OBJECT

protected:
	// this event is called, when a new translator is loaded or the system language is changed
	void changeEvent(QEvent*);

protected slots:
	// this slot is called by the language menu actions
	void slotLanguageChanged(QAction* action);

private:
	// loads a language by the given language shortcur (e.g. de, en)
	void loadLanguage(const QString& rLanguage);

	// creates the language menu dynamically from the content of m_langPath
	void createLanguageMenu(void);

public:
	explicit MainWindow(QWidget *parent = 0);
	~MainWindow();

	void setup( const QString &db_file );
	void reduce_mods_list( void );
	void build_mods_list( QVector<mod> &m );
	void clear_mods_found();
	void draw_new_weapon_1(weapon &wpn );
	void draw_new_weapon_burst(weapon &wpn );
	void draw_new_weapon_sust(weapon &wpn );
	void mods_forced_changed();

	bool open_db(const QString &path);
	void load_db( const QString &f );
	bool store_wpn( const weapon &wpn );
	bool update_wpn( const weapon &wpn );
	bool delete_wpn( const QString &name );
	bool store_mod( const mod &m );
	bool update_mod( const mod &m );
	bool delete_mod( const QString &name );

	static void wpn_calc_melee( const weapon &src, weapon &dst, const double *buff );
	static void wpn_calc_firing( const weapon &src, weapon &dst, const double *buff );
	static void wpn_calc( const weapon &src_wpn, weapon &dst_wpn, const double *buff );
	static void wpn_build( const weapon &src, const QVector<mod> &forced, const QVector<mod> &list, int n,
					int riven, bool neg, weapon &dst_1, weapon &dst_burst, weapon &dst_sust, MainWindow *self );
	static void wpn_riven(const weapon &src, const QVector<mod> &forced, const QVector<mod> &list, int n,
					int riven, bool neg, weapon &dst_1, weapon &dst_burst, weapon &dst_sust, MainWindow *self );

	template <typename tmember>
	static void wpn_calc_dps( const weapon &src, weapon &dst, const double *buff, const QVector<mod> &list,
							  int mods_count,uint64_t &mask, tmember member );
	template <typename tmember>
	static void wpn_calc_riven( const weapon &src, weapon &dst, const double *buff, const QVector<mod> &list, int mods_count, int riven,
								uint64_t &mask, tmember member, QVector<mod> &r_stat, QVector<mod> &mods, bool neg, MainWindow *self );

	typedef void( calc_func )( const weapon &, const QVector<mod> &, const QVector<mod> &, int, int, bool,
							   weapon &, weapon &, weapon &, MainWindow * );
	template <calc_func f>
	void calc_build_riven( int n );
	double parse_percent( const QString &str );
	QString printf( double v );
	QString printf( int v ) { return QString::number( v ); }
	void draw_mod_list( QListWidget *wgt, QVector<mod> &mods );
	void draw_mod( const mod &m );

private slots:

	void on_calc_build_clicked();
	void on_calc_mod_exclude_clicked();
	void on_calc_mod_include_clicked();
	void on_calc_mods_excluded_itemClicked(QListWidgetItem *item);
	void on_calc_mods_excluded_itemDoubleClicked(QListWidgetItem *item);
	void on_calc_mods_forced_itemClicked(QListWidgetItem *item);
	void on_calc_mods_forced_itemDoubleClicked(QListWidgetItem *item);
	void on_calc_mods_found_1_itemClicked(QListWidgetItem *item);
	void on_calc_mods_found_burst_itemClicked(QListWidgetItem *item);
	void on_calc_mods_found_sust_itemClicked(QListWidgetItem *item);
	void on_calc_mods_itemClicked(QListWidgetItem *item);
	void on_calc_mods_itemDoubleClicked(QListWidgetItem *item);
	void on_calc_riven_clicked( int n );
	void on_calc_weapon_currentIndexChanged(const QString &wpn_nm);
	void on_calc_wpn_filter_currentIndexChanged(const QString &arg1);

	void on_mod_delete_pressed();
	void on_mod_ena2_toggled(bool checked);
	void on_mod_ena3_toggled(bool checked);
	void on_mod_ena4_toggled(bool checked);
	void on_mod_exact_weapon_currentIndexChanged( const QString &name );
	void on_mod_list_itemClicked(QListWidgetItem *item);
	void on_mod_list_itemSelectionChanged();
	void on_mod_save_clicked();
	void on_mod_type_currentIndexChanged(int index);
	void on_mod_weapon_toggled(bool checked);

	void on_wpn_delete_pressed();
	void on_wpn_elemental_currentIndexChanged(int index);
	void on_wpn_list_itemClicked(QListWidgetItem *item);
	void on_wpn_list_itemSelectionChanged();
	void on_wpn_save_clicked();
	void on_wpn_type_currentIndexChanged(int index);

	void on_calc_riven2_clicked();

	void on_calc_riven3_clicked();

	void on_calc_mods_riven_1_itemClicked(QListWidgetItem *item);

	void on_calc_mods_riven_burst_itemClicked(QListWidgetItem *item);

	void on_calc_mods_riven_sust_itemClicked(QListWidgetItem *item);

private:
	Ui::MainWindow *ui;
	QSqlDatabase db;
	QTranslator m_translator; // contains the translations for this application
	QTranslator m_translatorQt; // contains the translations for qt
	QString m_currLang; // contains the currently loaded language
	QString m_langPath; // Path of language files. This is always fixed to /languages.

	QStringList weapons_list, mods_list;

	QVector<weapon> weapons;

	QVector<mod> mods, mods_forced, mods_excluded;//, mods_1, mods_burst, mods_sust;

	QLabel *results[3][3];
};

#endif // MAINWINDOW_H
