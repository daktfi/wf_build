#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "QDebug"
#include "QDir"
#include "QSqlError"
#include "QMessageBox"
#include "constants.h"

#define lull uint64_t( 1 )

MainWindow::MainWindow(QWidget *parent) :
	QMainWindow(parent),
	ui(new Ui::MainWindow)
{
	ui->setupUi(this);
}

MainWindow::~MainWindow()
{
	delete ui;

	db.commit();
	db.close();
}

void MainWindow::setup(const QString &db_file )
{
	load_db( db_file );

	results[0][0] = ui->calc_lbl_1_1;
	results[0][1] = ui->calc_lbl_1_burst;
	results[0][2] = ui->calc_lbl_1_sust;
	results[1][0] = ui->calc_lbl_burst_1;
	results[1][1] = ui->calc_lbl_burst_burst;
	results[1][2] = ui->calc_lbl_burst_sust;
	results[2][0] = ui->calc_lbl_sust_1;
	results[2][1] = ui->calc_lbl_sust_burst;
	results[2][2] = ui->calc_lbl_sust_sust;

	damage[0] = ui->calc_grineer_cloned_val;
	damage[1] = ui->calc_grineer_ferrite_val;
	damage[2] = ui->calc_grineer_alloy_val;
	damage[3] = ui->calc_grineer_machinery_val;
	damage[4] = ui->calc_corpus_flesh_val;
	damage[5] = ui->calc_corpus_shield_val;
	damage[6] = ui->calc_corpus_proto_val;
	damage[7] = ui->calc_corpus_robotic_val;
	damage[8] = ui->calc_infested_infested_val;
	damage[9] = ui->calc_infested_flesh_val;
	damage[10] = ui->calc_infested_fossil_val;
	damage[11] = ui->calc_infested_sinew_val;

	ui->calc_impact->clear();
	ui->calc_puncture->clear();
	ui->calc_slash->clear();
	ui->calc_element->clear();
	ui->calc_ele_dmg->clear();
	ui->calc_crit->clear();
	ui->calc_mult->clear();
	ui->calc_rof->clear();
	ui->calc_mag->clear();
	ui->calc_reload->clear();
	ui->calc_ammo->clear();
	ui->calc_proc->clear();
	ui->calc_new_impact_1->clear();
	ui->calc_new_puncture_1->clear();
	ui->calc_new_slash_1->clear();
	ui->calc_new_ele_dmg_1->clear();
	ui->calc_new_crit_1->clear();
	ui->calc_new_mult_1->clear();
	ui->calc_new_rof_1->clear();
	ui->calc_new_mag_1->clear();
	ui->calc_new_reload_1->clear();
	ui->calc_new_proc_1->clear();
	ui->calc_new_impact_burst->clear();
	ui->calc_new_puncture_burst->clear();
	ui->calc_new_slash_burst->clear();
	ui->calc_new_ele_dmg_burst->clear();
	ui->calc_new_crit_burst->clear();
	ui->calc_new_mult_burst->clear();
	ui->calc_new_rof_burst->clear();
	ui->calc_new_mag_burst->clear();
	ui->calc_new_reload_burst->clear();
	ui->calc_new_proc_burst->clear();
	ui->calc_new_impact_sust->clear();
	ui->calc_new_puncture_sust->clear();
	ui->calc_new_slash_sust->clear();
	ui->calc_new_ele_dmg_sust->clear();
	ui->calc_new_crit_sust->clear();
	ui->calc_new_mult_sust->clear();
	ui->calc_new_rof_sust->clear();
	ui->calc_new_mag_sust->clear();
	ui->calc_new_reload_sust->clear();
	ui->calc_new_proc_sust->clear();
	ui->calc_dispo->clear();
	ui->calc_mod_desc->clear();
	ui->calc_mod_name->clear();
	ui->calc_mod_stat1->clear();
	ui->calc_mod_stat2->clear();
	ui->calc_mod_stat3->clear();
	ui->calc_mod_stat4->clear();
	ui->calc_mod_val1->clear();
	ui->calc_mod_val2->clear();
	ui->calc_mod_val3->clear();
	ui->calc_mod_val4->clear();
	ui->calc_weapon->addItems( weapons_list );
	ui->calc_wpn_filter->clear();
	ui->calc_wpn_filter->addItem( "Any" );
	ui->calc_wpn_filter->addItems( weapon_type_names );
	ui->calc_mods_riven_1->clear();
	ui->calc_mods_riven_burst->clear();
	ui->calc_mods_riven_sust->clear();
	ui->calc_grineer->setChecked( false );
	ui->calc_corpus->setChecked( false );
	ui->calc_infested->setChecked( false );

	ui->mod_type->addItems( weapon_type_names );
	ui->mod_type->addItem( "Primary" );
	ui->mod_type->setCurrentText( weapon_type_names.at( wpn_rifle ) );
	ui->mod_stat1->addItems( stats_firing );
	ui->mod_stat2->addItems( stats_firing );
	ui->mod_list->addItems( mods_list );
	ui->mod_exact_weapon->addItems( weapons_list );

	ui->wpn_type->clear();
	ui->wpn_type->addItems( weapon_type_names );
	ui->wpn_elemental->addItems( element_names );
	ui->wpn_list->addItems( weapons_list );
}

void MainWindow::on_mod_type_currentIndexChanged(int index)
{
	ui->mod_subtype->setDisabled( false );
	ui->mod_subtype->clear();
	ui->mod_stat1->clear();
	ui->mod_stat2->clear();
	ui->mod_stat3->clear();
	ui->mod_stat4->clear();

	on_mod_stat_enabled( ui->mod_stat1 );
	on_mod_stat_enabled( ui->mod_stat2 );
	on_mod_stat_enabled( ui->mod_stat3 );
	on_mod_stat_enabled( ui->mod_stat4 );

	switch( index )
	{
	case wpn_rifle:
		ui->mod_subtype->addItems( rifle_subtypes );
		break;
	case wpn_shotgun:
		ui->mod_subtype->addItems( shotgun_subtypes );
		break;
	case wpn_secondary:
		ui->mod_subtype->addItems( secondary_subtypes );
		break;
	case wpn_melee:
		ui->mod_subtype->addItems( melee_subtypes );
		break;
	default:	// Archwing weapons and "any primary" mods have no subtypes
		ui->mod_subtype->setDisabled( true );
		break;
	}
}

void MainWindow::on_wpn_type_currentIndexChanged(int index)
{
	ui->wpn_subtype->clear();
	ui->wpn_mag->setDisabled( index == wpn_melee || index == wpn_archmelee );
	ui->wpn_ammo->setDisabled( index == wpn_melee || index == wpn_archmelee );
	ui->wpn_reload->setDisabled( index == wpn_melee || index == wpn_archmelee );
	ui->wpn_regen->setDisabled( index == wpn_melee || index == wpn_archmelee );
	ui->wpn_rnd->setDisabled( index == wpn_melee || index == wpn_archmelee );
	ui->wpn_pellets->setDisabled( index == wpn_melee || index == wpn_archmelee );

	switch( index )
	{
	case wpn_rifle:
		ui->wpn_subtype->addItems( rifle_subtypes );
		break;
	case wpn_shotgun:
		ui->wpn_subtype->addItems( shotgun_subtypes );
		break;
	case wpn_secondary:
		ui->wpn_subtype->addItems( secondary_subtypes );
		break;
	case wpn_melee:
		ui->wpn_subtype->addItems( melee_subtypes );
		break;
	default:
		break;
	}
}


void MainWindow::on_mod_weapon_toggled(bool checked)
{
	ui->mod_exact_weapon->clear();
	ui->mod_exact_weapon->addItems( weapons_list );
	ui->mod_exact_weapon->setDisabled( !checked );
}

void MainWindow::on_wpn_save_clicked()
{
	weapon wpn;
	wpn.name = ui->wpn_name->text();
	wpn.impact = ui->wpn_impact->text().toDouble();
	wpn.puncture = ui->wpn_puncture->text().toDouble();
	wpn.slash = ui->wpn_slash->text().toDouble();
	wpn.element = elements( ui->wpn_elemental->currentIndex() );
	wpn.elemental = ui->wpn_ele_dmg->text().toDouble();
	wpn.crit = parse_percent( ui->wpn_crit->text() );
	wpn.mult = ui->wpn_mult->text().toDouble();
	wpn.proc = parse_percent( ui->wpn_proc->text() );
	wpn.type = ui->wpn_type->currentIndex();
	wpn.subtype = ui->wpn_subtype->currentIndex();
	wpn.dispo = ui->wpn_dispo->text().toDouble();
	wpn.desc = ui->wpn_desc->text();
	wpn.rnd = ui->wpn_rnd->text().toInt();
	wpn.rof = ui->wpn_rof->text().toDouble();
	wpn.pellets = ui->wpn_pellets->text().toInt();
	wpn.regen = ui->wpn_regen->text().toDouble();

	if( wpn.type != wpn_melee && wpn.type != wpn_archmelee ) {
		wpn.reload = ui->wpn_reload->text().toDouble();
		wpn.magazine = ui->wpn_mag->text().toInt();
		wpn.ammo = ui->wpn_ammo->text().toInt();
	} else
		wpn.reload = wpn.magazine = wpn.ammo = 0;

	if( !wpn.name.isEmpty() ) {
		for( auto &w : weapons )
			if( wpn.name == w.name ) {
				if( !update_wpn( wpn ) )
					qDebug() << "Error updating db!";

				w = wpn;

				return;
			}

		if( !store_wpn( wpn ) )
			qDebug() << "Error storing to db!";

		weapons << wpn;
		weapons_list << wpn.name;

		ui->calc_weapon->clear();
		ui->calc_weapon->addItems( weapons_list );

		ui->mod_exact_weapon->clear();
		ui->mod_exact_weapon->addItems( weapons_list );

		ui->wpn_list->clear();
		ui->wpn_list->addItems( weapons_list );
	}
}

void MainWindow::on_mod_stat_enabled( QComboBox *cb )
{
	cb->clear();

	switch( ui->mod_type->currentIndex() )
	{
	case wpn_rifle:
	case wpn_shotgun:
	case wpn_secondary:
	case wpn_archgun:
	case wpn_primary:
		cb->addItems( stats_firing );
		break;
	case wpn_melee:
	case wpn_archmelee:
		cb->addItems( stats_melee );
		break;
	}
}

void MainWindow::on_mod_ena2_toggled(bool checked)
{
	ui->mod_stat2->setDisabled( !checked );
	ui->mod_val2->setDisabled( !checked );
	ui->mod_ena3->setDisabled( !checked );

	if( !checked ) {
		ui->mod_ena3->setChecked( false );
		ui->mod_ena4->setChecked( false );
	}

	on_mod_stat_enabled( ui->mod_stat2 );
}

void MainWindow::on_mod_ena3_toggled(bool checked)
{
	ui->mod_stat3->setDisabled( !checked );
	ui->mod_val3->setDisabled( !checked );
	ui->mod_ena4->setDisabled( !checked );

	if( !checked )
		ui->mod_ena4->setChecked( false );

	on_mod_stat_enabled( ui->mod_stat3 );
}

void MainWindow::on_mod_ena4_toggled(bool checked)
{
	ui->mod_stat4->setDisabled( !checked );
	ui->mod_val4->setDisabled( !checked );

	on_mod_stat_enabled( ui->mod_stat4 );
}

void MainWindow::on_wpn_list_itemClicked(QListWidgetItem *item)
{
	for( const auto &w : weapons )
		if( w.name == item->text() ) {
			ui->wpn_name->setText( w.name );
			ui->wpn_type->setCurrentIndex( w.type );
			ui->wpn_subtype->setCurrentIndex( w.subtype );
			ui->wpn_impact->setText( printf( w.impact ) );
			ui->wpn_puncture->setText( printf( w.puncture ) );
			ui->wpn_slash->setText( printf( w.slash ) );
			ui->wpn_elemental->setCurrentIndex( w.element );
			ui->wpn_ele_dmg->setText( printf( w.elemental ) );
			ui->wpn_crit->setText( printf( w.crit * 100.0 ) + "%" );
			ui->wpn_mult->setText( printf( w.mult ) );
			ui->wpn_proc->setText( printf( w.proc * 100.0 ) + "%" );
			ui->wpn_dispo->setText( printf( w.dispo ) );
			ui->wpn_rof->setText( printf( w.rof ) );
			ui->wpn_rnd->setText( printf( w.rnd ) );
			ui->wpn_pellets->setText( printf( w.pellets ) );
			ui->wpn_mag->setText( printf( w.magazine ) );
			ui->wpn_reload->setText( printf( w.reload ) );
			ui->wpn_ammo->setText( printf( w.ammo ) );
			ui->wpn_desc->setText( w.desc );
			ui->wpn_regen->setText( printf( w.regen ) );

			return;
		}
}

void MainWindow::on_wpn_elemental_currentIndexChanged(int index)
{
	ui->wpn_ele_dmg->setDisabled( !index );

	if( !index )
		ui->wpn_ele_dmg->setText( "0" );
}

void MainWindow::on_mod_save_clicked()
{
	mod m;
	m.name = ui->mod_name->text();
	m.type = ui->mod_type->currentIndex();
	m.subtype = ui->mod_subtype->currentIndex();
	m.stat1 = ui->mod_stat1->currentIndex();
	m.val1 = ui->mod_val1->text().toDouble();
	m.desc = ui->mod_desc->text();

	if( ui->mod_weapon->isChecked() )
		m.exact_wpn = ui->mod_exact_weapon->currentText();

	if( ui->mod_ena2->isChecked() ) {
		m.stat2 = ui->mod_stat2->currentIndex();
		m.val2 = ui->mod_val2->text().toDouble();
	} else {
		m.stat2 = -1;
		m.val2 = 0;
	}

	if( ui->mod_ena3->isChecked() ) {
		m.stat3 = ui->mod_stat3->currentIndex();
		m.val3 = ui->mod_val3->text().toDouble();
	} else {
		m.stat3 = -1;
		m.val3 = 0;
	}

	if( ui->mod_ena4->isChecked() ) {
		m.stat4 = ui->mod_stat4->currentIndex();
		m.val4 = ui->mod_val4->text().toDouble();
	} else {
		m.stat4 = -1;
		m.val4 = 0;
	}

	if( !m.name.isEmpty() ) {
		for( auto it = mods.begin(); it != mods.end(); ++it ) {
			if( m.name == it->name ) {
				if( !update_mod( m ) )
					qDebug() << "Error updating db!";

				*it = m;

				return;
			}
		}

		if( !store_mod( m ) )
			qDebug() << "Error storing to db!";

		mods << m;
		mods_list << m.name;

		reduce_mods_list();

		ui->mod_list->clear();
		ui->mod_list->addItems( mods_list );

	}
}

void MainWindow::on_mod_list_itemClicked( QListWidgetItem *item )
{
	for( const auto &m : mods )
		if( m.name == item->text() ) {
			ui->mod_name->setText( m.name );
			ui->mod_desc->setText( m.desc );
			ui->mod_weapon->setChecked( !m.exact_wpn.isEmpty() );
			ui->mod_type->setCurrentIndex( m.type );
			ui->mod_subtype->setCurrentIndex( m.subtype );

			ui->mod_stat1->setCurrentIndex( m.stat1 );
			ui->mod_val1->setText( printf( m.val1 ) );
			ui->mod_ena2->setChecked( m.stat2 >= 0 );
			ui->mod_ena3->setChecked( m.stat3 >= 0 );
			ui->mod_ena4->setChecked( m.stat4 >= 0 );

			if( m.stat2 >= 0 ) {
				ui->mod_stat2->setCurrentIndex( m.stat2 );
				ui->mod_val2->setText( printf( m.val2 ) );
			} else {
				ui->mod_stat2->setCurrentIndex( 0 );
				ui->mod_val2->setText( "0" );
			}

			if( m.stat3 >= 0 ) {
				ui->mod_stat3->setCurrentIndex( m.stat3 );
				ui->mod_val3->setText( printf( m.val3 ) );
			} else {
				ui->mod_stat3->setCurrentIndex( 0 );
				ui->mod_val3->setText( "0" );
			}

			if( m.stat4 >= 0 ) {
				ui->mod_stat4->setCurrentIndex( m.stat4 );
				ui->mod_val4->setText( printf( m.val4 ) );
			} else {
				ui->mod_stat4->setCurrentIndex( 0 );
				ui->mod_val4->setText( "0" );
			}

			if( !m.exact_wpn.isEmpty() )
				ui->mod_exact_weapon->setCurrentText( m.exact_wpn );

			return;
		}
}

void MainWindow::reduce_mods_list()
{
	const QString wpn_nm = ui->calc_weapon->currentText();

	for( const auto &w : weapons )
		if( wpn_nm == w.name ) {
			ui->calc_mods->clear();

			for( const auto &m : mods )
				if( ( m.type == w.type && ( m.subtype == wpn_normal || m.subtype == w.subtype ) && ( m.exact_wpn.isEmpty() || m.exact_wpn == w.name ) )
						|| ( m.type == wpn_primary && ( w.type == wpn_rifle || w.type == wpn_shotgun ) ) )
					ui->calc_mods->addItem( m.name );

			return;
		}
}

void MainWindow::on_calc_weapon_currentIndexChanged( const QString &wpn_nm )
{
	mods_forced.clear();
	mods_excluded.clear();
	ui->calc_mods_forced->clear();
	ui->calc_mods_excluded->clear();

	reduce_mods_list();
	clear_mods_found();
	mods_forced_changed();

	for( const auto &w : weapons )
		if( w.name == wpn_nm ) {
			ui->calc_desc->setText( w.desc );
			ui->calc_type->setText( weapon_type_names.at( w.type ) );
			ui->calc_impact->setText( printf( w.impact ) );
			ui->calc_puncture->setText( printf( w.puncture ) );
			ui->calc_slash->setText( printf( w.slash ) );
			ui->calc_crit->setText( printf( w.crit * 100.0 ) + "%" );
			ui->calc_mult->setText( printf( w.mult ) );
			ui->calc_proc->setText( printf( w.proc * 100.0 ) + "%" );
			ui->calc_dispo->setText( printf( w.dispo ) );
			ui->calc_rof->setText( printf( w.rof ) );
			ui->calc_rnd->setText( printf( w.rnd ) );
			ui->calc_pellets->setText( printf( w.pellets ) );
			ui->calc_mag->setText( printf( w.magazine ) );
			ui->calc_reload->setText( printf( w.reload ) );
			ui->calc_ammo->setText( printf( w.ammo ) );
			ui->calc_riven2->setDisabled( w.type == wpn_archgun || w.type == wpn_archmelee );
			ui->calc_riven3->setDisabled( w.type == wpn_archgun || w.type == wpn_archmelee );
			ui->calc_riven_neg->setDisabled( w.type == wpn_archgun || w.type == wpn_archmelee );

			if( w.element != element_none ) {
				ui->calc_element->setText( element_names.at( w.element ) );
				ui->calc_ele_dmg->setText( printf( w.elemental ) );
			} else {
				ui->calc_element->setText( "Elemental" );
				ui->calc_ele_dmg->setText( "none" );
			}

			if( w.type == wpn_melee || w.type == wpn_archmelee ) {
				ui->calc_result->setTabText( 0, "Combo x1" );
				ui->calc_result->setTabText( 1, "Combo x2" );
				ui->calc_result->setTabText( 2, "Combo x3" );

				for( int i = 0; i < 3; ++i ) {
					results[i][0]->setText( "Combo x1 DPS" );
					results[i][1]->setText( "Combo x2 DPS" );
					results[i][2]->setText( "Combo x3 DPS" );
				}
			} else {
				ui->calc_result->setTabText( 0, "Single Shot" );
				ui->calc_result->setTabText( 1, "Burst DPS" );
				ui->calc_result->setTabText( 2, "Sustained DPS" );

				for( int i = 0; i < 3; ++i ) {
					results[i][0]->setText( "Single Shot" );
					results[i][1]->setText( "Burst DPS" );
					results[i][2]->setText( "Sustained DPS" );
				}
			}

			switch( w.type )
			{
			case wpn_rifle:
				ui->calc_subtype->setText( rifle_subtypes.at( w.subtype ) );
				break;
			case wpn_shotgun:
				ui->calc_subtype->setText( shotgun_subtypes.at( w.subtype ) );
				break;
			case wpn_secondary:
				ui->calc_subtype->setText( secondary_subtypes.at( w.subtype ) );
				break;
			case wpn_melee:
				ui->calc_subtype->setText( melee_subtypes.at( w.subtype ) );
			}

			return;
		}
}

void MainWindow::on_mod_delete_pressed()
{
	QString name = ui->mod_name->text();

	if( delete_mod( name ) ) {
		for( auto it = mods.begin(); it != mods.end(); ++it )
			if( it->name == name ) {
				mods.erase( it );
				break;
			}

		for( auto it = mods_list.begin(); it != mods_list.end(); ++it )
			if( *it == name ) {
				mods_list.erase( it );
				ui->mod_list->clear();
				ui->mod_list->addItems( mods_list );
				return;
			}
	}
}

void MainWindow::on_wpn_delete_pressed()
{
	QString name = ui->wpn_name->text();

	if( delete_wpn( ui->wpn_name->text() ) ) {
		for( auto it = weapons.begin(); it != weapons.end(); ++it )
			if( it->name == name ) {
				weapons.erase( it );
				break;
			}

		for( auto it = weapons_list.begin(); it != weapons_list.end(); ++it )
			if( *it == name ) {
				weapons_list.erase( it );
				ui->wpn_list->clear();
				ui->wpn_list->addItems( weapons_list );
				ui->calc_weapon->clear();
				ui->calc_weapon->addItems( weapons_list );
				return;
			}
	}
}


void MainWindow::on_calc_mods_itemClicked(QListWidgetItem *item)
{
	const QString name = item->text();

	for( const auto &m : mods )
		if( name == m.name ) {
			draw_mod( m );
			return;
		}
}

void MainWindow::on_calc_mods_itemDoubleClicked(QListWidgetItem *item)
{
	const QString name = item->text();

	if( ui->calc_mods_forced->count() >= ui->calc_mods_count->value() )
		return;

	if( mods_forced.size() < ui->calc_mods_count->value() ) {
		for( auto &it : mods_forced )
			if( name == it.name )
				return;

		for( int row = 0; row < ui->calc_mods_excluded->count(); ++row )
			if( name == ui->calc_mods_excluded->item( row )->text() ) {
				on_calc_mods_excluded_itemDoubleClicked( ui->calc_mods_excluded->item( row ) );
				break;
			}

		for( const auto &m : mods )
			if( name == m.name ) {
				mods_forced << m;
				ui->calc_mods_forced->addItem( m.name );
				mods_forced_changed();
				return;
			}
	}
}

void MainWindow::on_calc_mod_include_clicked()
{
	if( ui->calc_mods->currentItem() )
		on_calc_mods_itemDoubleClicked( ui->calc_mods->currentItem() );
}

void MainWindow::on_calc_mod_exclude_clicked()
{
	const QString name = ui->calc_mod_name->text();

	for( const auto &m : mods_excluded )
		if( name == m.name )
			return;

	for( int row = 0; row < ui->calc_mods_forced->count(); ++row )
		if( name == ui->calc_mods_forced->item( row )->text() ) {
			on_calc_mods_forced_itemDoubleClicked( ui->calc_mods_forced->item( row ) );
			break;
		}

	for( const auto &m : mods )
		if( name == m.name ) {
			mods_excluded << m;
			ui->calc_mods_excluded->addItem( m.name );
		}
}

void MainWindow::on_calc_mods_forced_itemDoubleClicked(QListWidgetItem *item)
{
	const QString name = item->text();

	for( auto it = mods_forced.begin(); it != mods_forced.end(); ++it )
		if( name == it->name ) {
			mods_forced.erase( it );
			delete item;
			mods_forced_changed();
			return;
		}
}

void MainWindow::on_calc_mods_excluded_itemDoubleClicked(QListWidgetItem *item)
{
	const QString name = item->text();

	for( auto it = mods_excluded.begin(); it != mods_excluded.end(); ++it )
		if( name == it->name ) {
			mods_excluded.erase( it );
			delete item;
			return;
		}
}

void MainWindow::mods_forced_changed()
{
	const QString name = ui->calc_weapon->currentText();

	for( const auto &it : weapons )
		if( name == it.name ) {
			weapon result;
			wpn_build( it, mods_forced, QVector<mod>(), ui->calc_mods_count->value(), 0, false, result, result, result, nullptr );
			draw_new_weapon_1( result );
			draw_new_weapon_burst( result );
			draw_new_weapon_sust( result );
			return;
		}
}

void MainWindow::draw_new_weapon_1( weapon &wpn )
{
	ui->calc_new_impact_1->setText( printf( wpn.impact ) );
	ui->calc_new_puncture_1->setText( printf( wpn.puncture ) );
	ui->calc_new_slash_1->setText( printf( wpn.slash ) );
	ui->calc_new_ele_dmg_1->setText( printf( wpn.elemental ) );
	ui->calc_new_crit_1->setText( printf( wpn.crit * 100.0 ) + "%" );
	ui->calc_new_mult_1->setText( printf( wpn.mult ) );
	ui->calc_new_rof_1->setText( printf( wpn.rof ) );
	ui->calc_new_mag_1->setText( printf( wpn.magazine ) );
	ui->calc_new_reload_1->setText( printf( wpn.reload ) );
	ui->calc_new_proc_1->setText( printf( wpn.proc * 100.0 ) + "%" );

	double phy = wpn.impact + wpn.puncture + wpn.slash, ele = wpn.elemental, total = phy + ele;

	if( total > 0 ) {
		ui->calc_dmg_ele_1->setText( printf( 100 * ele / total ) + "%" );
		ui->calc_dmg_phy_1->setText( printf( 100 * ( total - ele ) / total ) + "%" );
	}

	if( wpn.type == wpn_melee ||wpn.type == wpn_archmelee ) {
		ui->calc_new_pellets_1->clear();
		ui->calc_dps_1->setText( printf( wpn.dps_x10 ) );
		ui->calc_dps_1_burst->setText( printf( wpn.dps_x20 ) );
		ui->calc_dps_1_sust->setText( printf( wpn.dps_x30 ) );
	} else {
		ui->calc_new_pellets_1->setText( printf( wpn.pellets ) );
		ui->calc_dps_1->setText( printf( wpn.dps_1 ) );
		ui->calc_dps_1_burst->setText( printf( wpn.dps_burst ) );
		ui->calc_dps_1_sust->setText( printf( wpn.dps_sust ) );
	}
}

void MainWindow::draw_new_weapon_burst( weapon &wpn )
{
	ui->calc_new_impact_burst->setText( printf( wpn.impact ) );
	ui->calc_new_puncture_burst->setText( printf( wpn.puncture ) );
	ui->calc_new_slash_burst->setText( printf( wpn.slash ) );
	ui->calc_new_ele_dmg_burst->setText( printf( wpn.elemental ) );
	ui->calc_new_crit_burst->setText( printf( wpn.crit * 100.0 ) + "%" );
	ui->calc_new_mult_burst->setText( printf( wpn.mult ) );
	ui->calc_new_rof_burst->setText( printf( wpn.rof ) );
	ui->calc_new_mag_burst->setText( printf( wpn.magazine ) );
	ui->calc_new_reload_burst->setText( printf( wpn.reload ) );
	ui->calc_new_proc_burst->setText( printf( wpn.proc * 100.0 ) + "%" );

	double phy = wpn.impact + wpn.puncture + wpn.slash, ele = wpn.elemental, total = phy + ele;

	if( total > 0 ) {
		ui->calc_dmg_ele_burst->setText( printf( 100 * ele / total ) + "%" );
		ui->calc_dmg_phy_burst->setText( printf( 100 * ( total - ele ) / total ) + "%" );
	}

	if( wpn.type == wpn_melee || wpn.type == wpn_archmelee ) {
		ui->calc_new_pellets_burst->clear();
		ui->calc_dps_burst_1->setText( printf( wpn.dps_x10 ) );
		ui->calc_dps_burst->setText( printf( wpn.dps_x20 ) );
		ui->calc_dps_burst_sust->setText( printf( wpn.dps_x30 ) );
	} else {
		ui->calc_new_pellets_burst->setText( printf( wpn.pellets ) );
		ui->calc_dps_burst_1->setText( printf( wpn.dps_1 ) );
		ui->calc_dps_burst->setText( printf( wpn.dps_burst ) );
		ui->calc_dps_burst_sust->setText( printf( wpn.dps_sust ) );
	}
}

void MainWindow::draw_new_weapon_sust( weapon &wpn )
{
	ui->calc_new_impact_sust->setText( printf( wpn.impact ) );
	ui->calc_new_puncture_sust->setText( printf( wpn.puncture ) );
	ui->calc_new_slash_sust->setText( printf( wpn.slash ) );
	ui->calc_new_ele_dmg_sust->setText( printf( wpn.elemental ) );
	ui->calc_new_crit_sust->setText( printf( wpn.crit * 100.0 ) + "%" );
	ui->calc_new_mult_sust->setText( printf( wpn.mult ) );
	ui->calc_new_rof_sust->setText( printf( wpn.rof ) );
	ui->calc_new_mag_sust->setText( printf( wpn.magazine ) );
	ui->calc_new_reload_sust->setText( printf( wpn.reload ) );
	ui->calc_new_proc_sust->setText( printf( wpn.proc * 100.0 ) + "%" );

	double phy = wpn.impact + wpn.puncture + wpn.slash, ele = wpn.elemental, total = phy + ele;

	if( total > 0 ) {
		ui->calc_dmg_ele_sust->setText( printf( 100 * ele / total ) + "%" );
		ui->calc_dmg_phy_sust->setText( printf( 100 * ( total - ele ) / total ) + "%" );
	}

	if( wpn.type == wpn_melee || wpn.type == wpn_archmelee ) {
		ui->calc_new_pellets_sust->clear();
		ui->calc_dps_sust_1->setText( printf( wpn.dps_x10 ) );
		ui->calc_dps_sust_burst->setText( printf( wpn.dps_x20 ) );
		ui->calc_dps_sust->setText( printf( wpn.dps_x30 ) );
	} else {
		ui->calc_new_pellets_sust->setText( printf( wpn.pellets ) );
		ui->calc_dps_sust_1->setText( printf( wpn.dps_1 ) );
		ui->calc_dps_sust_burst->setText( printf( wpn.dps_burst ) );
		ui->calc_dps_sust->setText( printf( wpn.dps_sust ) );
	}
}

QString MainWindow::printf( double v )
{
	QString t;

	if( std::abs( v ) >= 1000 )
		t.sprintf( "%0.0lf", v );
	else if( std::abs( v ) >= 100 )
		t.sprintf( "%0.1lf", v );
	else if( std::abs( v ) < 0.1 )
		t.sprintf( "%0.3lf", v );
	else
		t.sprintf( "%0.2lf", v );

	return t;
}

void MainWindow::on_calc_mods_excluded_itemClicked(QListWidgetItem *item)
{
	on_calc_mods_itemClicked( item );
}

void MainWindow::on_calc_mods_forced_itemClicked(QListWidgetItem *item)
{
	on_calc_mods_itemClicked( item );
}

template <MainWindow::calc_func f>
void MainWindow::calc_build_riven( int n )
{
	QVector<mod> m;
	const QString name = ui->calc_weapon->currentText();

	build_mods_list( m );
	clear_mods_found();

	ui->calc_riven_progress->setValue( 0 );
	ui->calc_mods_riven_1->clear();
	ui->calc_mods_riven_burst->clear();
	ui->calc_mods_riven_sust->clear();

	for( const auto &it : weapons )
		if( name == it.name ) {
			weapon dst_1, dst_burst, dst_sust;

			f( it, mods_forced, m, ui->calc_mods_count->value(), n, ui->calc_riven_neg->isChecked(), dst_1, dst_burst, dst_sust, this );

			draw_mod_list( ui->calc_mods_found_1, mods_1 );
			draw_mod_list( ui->calc_mods_found_burst, mods_burst );
			draw_mod_list( ui->calc_mods_found_sust, mods_sust );

			draw_new_weapon_1( dst_1 );
			draw_new_weapon_burst( dst_burst );
			draw_new_weapon_sust( dst_sust );

			return;
		}
}

void MainWindow::on_calc_build_clicked()
{
	// Find best builds for single shot, burst dps and sustained dps.
	ui->calc_riven_progress->setDisabled( true );
	ui->calc_mods_riven_1->setDisabled( true );
	ui->calc_mods_riven_burst->setDisabled( true );
	ui->calc_mods_riven_sust->setDisabled( true );

	calc_build_riven<MainWindow::wpn_build>( 0 );
}

void MainWindow::on_calc_riven_clicked( int n )
{
	// Find best builds for single shot, burst dps and sustained dps.
	ui->calc_riven_progress->setDisabled( false );
	ui->calc_mods_riven_1->setDisabled( false );
	ui->calc_mods_riven_burst->setDisabled( false );
	ui->calc_mods_riven_sust->setDisabled( false );

	calc_build_riven<MainWindow::wpn_riven>( n );
}

void MainWindow::clear_mods_found()
{
	mods_1.clear();
	mods_burst.clear();
	mods_sust.clear();

	ui->calc_mods_found_1->clear();
	ui->calc_mods_found_burst->clear();
	ui->calc_mods_found_sust->clear();
	ui->calc_mods_riven_1->clear();
	ui->calc_mods_riven_burst->clear();
	ui->calc_mods_riven_sust->clear();
}

void MainWindow::on_wpn_list_itemSelectionChanged()
{
	on_wpn_list_itemClicked( ui->wpn_list->currentItem() );
}

void MainWindow::on_mod_list_itemSelectionChanged()
{
	on_mod_list_itemClicked( ui->mod_list->currentItem() );
}

void MainWindow::on_calc_mods_found_1_itemClicked(QListWidgetItem *item)
{
	on_calc_mods_itemClicked( item );
}

void MainWindow::on_calc_mods_found_burst_itemClicked(QListWidgetItem *item)
{
	on_calc_mods_itemClicked( item );
}

void MainWindow::on_calc_mods_found_sust_itemClicked(QListWidgetItem *item)
{
	on_calc_mods_itemClicked( item );
}

void MainWindow::on_calc_wpn_filter_currentIndexChanged( const QString &s )
{
	ui->calc_weapon->clear();

	if( s == "Any" )
		for( const auto &w : weapons )
			ui->calc_weapon->addItem( w.name );
	else {
		int f;

		for( f = 0; f < weapon_type_names.size(); ++f )
			if( s == weapon_type_names[f] )
				break;

		for( const auto &w : weapons )
			if( w.type == f )
				ui->calc_weapon->addItem( w.name );
	}
}

// we create the menu entries dynamically, dependent on the existing translations.
void MainWindow::createLanguageMenu(void)
{
	QActionGroup* langGroup = new QActionGroup(ui->menuLanguage);
	langGroup->setExclusive(true);

	connect(langGroup, SIGNAL (triggered(QAction *)), this, SLOT (slotLanguageChanged(QAction *)));

	// format systems language
	QString defaultLocale = QLocale::system().name(); // e.g. "de_DE"
	defaultLocale.truncate(defaultLocale.lastIndexOf('_')); // e.g. "de"

	m_langPath = QApplication::applicationDirPath();
	QDir dir(m_langPath);
	QStringList fileNames = dir.entryList(QStringList("combo_*.qm"));

	for (int i = 0; i < fileNames.size(); ++i) {
		// get locale extracted by filename
		QString locale;
		locale = fileNames[i]; // "combo_de.qm"
		locale.truncate(locale.lastIndexOf('.')); // "combo_de"
		locale.remove(0, locale.indexOf('_') + 1); // "de"

		QString lang = QLocale::languageToString(QLocale(locale).language());
		QIcon ico(QString("%1/%2.png").arg(m_langPath).arg(locale));

		QAction *action = new QAction(ico, lang, this);
		action->setCheckable(true);
		action->setData(locale);

		ui->menuLanguage->addAction(action);
		langGroup->addAction(action);

		// set default translators and language checked
		if (defaultLocale == locale)
		{
			action->setChecked(true);
		}
	}
}

// Called every time, when a menu entry of the language menu is called
void MainWindow::slotLanguageChanged(QAction* action)
{
	if( action ) {
		// load the language dependant on the action content
		loadLanguage(action->data().toString());
		setWindowIcon(action->icon());
	}
}

void switchTranslator(QTranslator& translator, const QString& filename)
{
	// remove the old translator
	qApp->removeTranslator(&translator);

	// load the new translator
	QString path = QApplication::applicationDirPath();
	path.append("/languages/");
	if(translator.load(path + filename)) //Here Path and Filename has to be entered because the system didn't find the QM Files else
		qApp->installTranslator(&translator);
}

void MainWindow::loadLanguage(const QString& rLanguage)
{
	if(m_currLang != rLanguage) {
		m_currLang = rLanguage;
		QLocale locale = QLocale(m_currLang);
		QLocale::setDefault(locale);
		QString languageName = QLocale::languageToString(locale.language());
		switchTranslator(m_translator, QString("combo_%1.qm").arg(rLanguage));
		switchTranslator(m_translatorQt, QString("qt_%1.qm").arg(rLanguage));
		//ui->statusBar->showMessage(tr("Current Language changed to %1").arg(languageName));
	}
}

void MainWindow::changeEvent(QEvent* event)
{
	if( event ) {
		switch( event->type() )
		{
		// this event is send if a translator is loaded
		case QEvent::LanguageChange:
			ui->retranslateUi(this);
			break;

			// this event is send, if the system, language changes
		case QEvent::LocaleChange:
		{
			QString locale = QLocale::system().name();
			locale.truncate(locale.lastIndexOf('_'));
			loadLanguage(locale);
		}
			break;
		default:
			break;
		}
	}
	QMainWindow::changeEvent(event);
}

void MainWindow::build_mods_list( QVector<mod> &ml )
{
	const QString wpn_name = ui->calc_weapon->currentText();

	for( const auto &w : weapons )
		if( wpn_name == w.name ) {
			// Builds list of mods to use.
			for( const auto &m : mods )
				if( ( m.type == w.type )
						&& ( m.subtype == wpn_normal || m.subtype == w.subtype )
						&& ( m.exact_wpn.isEmpty() || m.exact_wpn == w.name ) ) {
					bool use = true;

					// Exclude forced mods (they're in other list)
					for( const auto &f : mods_forced )
						if( f.name == m.name ) {
							use = false;
							break;
						}

					// Exclude excluded mods
					if( use )
						for( const auto &x : mods_excluded )
							if( x.name == m.name ) {
								use = false;
								break;
							}

					// Exclude non-primed mod if primed is present
					if( use )
						for( auto &p : ml )
							if( m.name == "Primed " + p.name || p.name == "Primed " + m.name ) {
								if( m.name == "Primed " + p.name )
									p = m;

								use = false;
								break;
							}

					if( use )
						ml << m;
				}

			return;
		}
}

void MainWindow::on_mod_exact_weapon_currentIndexChanged( const QString &name )
{
	for( const auto &w : weapons )
		if( name == w.name ) {
			ui->mod_type->setCurrentIndex( w.type );
			ui->mod_subtype->setCurrentIndex( w.subtype );

			return;
		}
}

void MainWindow::load_db(const QString &f)
{
	if( !open_db( QFileInfo( f ).absoluteFilePath() ) )
		throw "Can't open database!";

	QSqlQuery query( db );

	if( !query.exec( "select wpn_name, impact, puncture, slash, element, ele_dmg,"
					 "crit, mult, status, rof, rnd, pellets, reload, magazine,"
					 "ammo, wpn_type, subtype, dispo, desc, regen from weapons "
					 "order by wpn_name") )
		qDebug() << "Can't read weapons! " << query.lastError().text();

	while( query.next() ) {
		weapon wpn( query );
		weapons_list << wpn.name;
		weapons << wpn;
	}

	if( !query.exec( "select mod_name, stat1, val1, stat2, val2, stat3, val3,"
					 "stat4, val4, mod_type, subtype, exact_wpn, desc from mods "
					 "order by mod_name") )
		qDebug() << "Can't read mods! " << query.lastError().text();

	while( query.next() ) {
		mod m( query );
		mods_list << m.name;
		mods << m;
	}
}

bool MainWindow::open_db( const QString &path )
{
	db = QSqlDatabase::addDatabase( "QSQLITE" );
	db.setDatabaseName( path );

	if (!db.open()) {
		QMessageBox::critical(nullptr, QObject::tr("Cannot open database"),
			QObject::tr("Unable to establish a database connection.\n"
						"This program needs SQLite support.\n\n"
						"Click Cancel to exit."), QMessageBox::Cancel);
		return false;
	}

	QSqlQuery query( db );

	if( !query.exec( "create table if not exists weapons (wpn_name varchar(80),"
					 "impact real, puncture real, slash real, element int,"
					 "ele_dmg real, crit real, mult real, status real, rof real,"
					 "rnd int, pellets int, reload real, magazine int, ammo int,"
					 "wpn_type int, subtype int, dispo real, desc varchar(250),"
					 "regen real)") )
		qDebug() << "Can't create weapons table! " << query.lastError().text();

	if( !query.exec( "create table if not exists mods (mod_name varchar(80),"
					 "stat1 int, val1 real, stat2 int, val2 real,stat3 int,"
					 "val3 real,stat4 int, val4 real, mod_type int,	"
					 "subtype int, exact_wpn varchar(80), desc varchar(250))") )
		qDebug() << "Can't create mods table! " << query.lastError().text();

	return true;
}

bool MainWindow::store_wpn( const weapon &wpn )
{
	QSqlQuery query( db );

	if( !query.prepare( "insert into weapons (wpn_name, impact, puncture, slash,"
						"element, ele_dmg, crit, mult, status, rof, rnd, pellets,"
						"reload, magazine, ammo, wpn_type, subtype, dispo, desc,"
						"regen) values (:name, :impact, :puncture, :slash, :element,"
						":ele_dmg, :crit, :mult, :proc, :rof, :rnd, :pellets,"
						":reload, :magazine, :ammo, :type, :subtype, :dispo,"
						":desc, :regen)") )
		qDebug() << "Error prepare " << query.lastError().text();

	query.bindValue( ":name", wpn.name );
	query.bindValue( ":impact", wpn.impact );
	query.bindValue( ":puncture", wpn.puncture );
	query.bindValue( ":slash", wpn.slash );
	query.bindValue( ":element", wpn.element );
	query.bindValue( ":ele_dmg", wpn.elemental );
	query.bindValue( ":crit", wpn.crit );
	query.bindValue( ":mult", wpn.mult );
	query.bindValue( ":proc", wpn.proc );
	query.bindValue( ":rof", wpn.rof );
	query.bindValue( ":rnd", wpn.rnd );
	query.bindValue( ":pellets", wpn.pellets );
	query.bindValue( ":reload", wpn.reload );
	query.bindValue( ":magazine", wpn.magazine );
	query.bindValue( ":ammo", wpn.ammo );
	query.bindValue( ":type", wpn.type );
	query.bindValue( ":subtype", wpn.subtype );
	query.bindValue( ":dispo", wpn.dispo );
	query.bindValue( ":desc", wpn.desc );
	query.bindValue( ":regen", wpn.regen );

	return query.exec();
}

bool MainWindow::update_wpn( const weapon &wpn )
{
	QSqlQuery query( db );

	if( !query.prepare( "update weapons set impact = :impact, puncture ="
						":puncture,	slash = :slash, element = :element, ele_dmg ="
						":ele_dmg, crit = :crit, mult = :mult, status = :proc,"
						"rof = :rof, rnd = :rnd, pellets = :pellets, reload ="
						":reload, magazine = :magazine, ammo = :ammo, wpn_type ="
						":type, subtype = :subtype, dispo = :dispo, desc = :desc,"
						"regen = :regen where wpn_name = :name") )
		qDebug() << "Can't prepare " << query.lastError().text();

	query.bindValue( ":name", wpn.name );
	query.bindValue( ":impact", wpn.impact );
	query.bindValue( ":puncture", wpn.puncture );
	query.bindValue( ":slash", wpn.slash );
	query.bindValue( ":element", wpn.element );
	query.bindValue( ":ele_dmg", wpn.elemental );
	query.bindValue( ":crit", wpn.crit );
	query.bindValue( ":mult", wpn.mult );
	query.bindValue( ":proc", wpn.proc );
	query.bindValue( ":rof", wpn.rof );
	query.bindValue( ":rnd", wpn.rnd );
	query.bindValue( ":pellets", wpn.pellets );
	query.bindValue( ":reload", wpn.reload );
	query.bindValue( ":magazine", wpn.magazine );
	query.bindValue( ":ammo", wpn.ammo );
	query.bindValue( ":type", wpn.type );
	query.bindValue( ":subtype", wpn.subtype );
	query.bindValue( ":dispo", wpn.dispo );
	query.bindValue( ":desc", wpn.desc );
	query.bindValue( ":regen", wpn.regen );

	return query.exec();
}

bool MainWindow::delete_wpn( const QString &name )
{
	QSqlQuery query( db );

	if( !query.prepare( "delete from weapons where wpn_name = :name" ) )
		qDebug() << "Can't prepare" << query.lastError().text();

	query.bindValue( ":name", name );
	return query.exec();
}

bool MainWindow::store_mod( const mod &m )
{
	QSqlQuery query( db );

	if( !query.prepare( "insert into mods (mod_name, stat1, val1, stat2, val2,"
						"stat3, val3, stat4, val4, mod_type, subtype, exact_wpn,"
						"desc) values (:name, :stat1, :val1, :stat2, :val2,"
						":stat3, :val3, :stat4, :val4, :type, :subtype, :exact,"
						":desc)") )
		qDebug() << "Error prepare " << query.lastError().text();

	query.bindValue( ":name", m.name );
	query.bindValue( ":stat1", m.stat1 );
	query.bindValue( ":val1", m.val1 );
	query.bindValue( ":stat2", m.stat2 );
	query.bindValue( ":val2", m.val2 );
	query.bindValue( ":stat3", m.stat3 );
	query.bindValue( ":val3", m.val3 );
	query.bindValue( ":stat4", m.stat4 );
	query.bindValue( ":val4", m.val4 );
	query.bindValue( ":type", m.type );
	query.bindValue( ":subtype", m.subtype );
	query.bindValue( ":exact", m.exact_wpn );
	query.bindValue( ":desc", m.desc );

	return query.exec();
}

bool MainWindow::update_mod( const mod &m )
{
	QSqlQuery query( db );

	if( !query.prepare( "update mods set stat1 = :stat1, val1 = :val1, stat2 ="
						":stat2, val2	= :val2, stat3 = :stat3, val3 = :val3,"
						"stat4 = :stat4, val4 = :val4, mod_type = :type,"
						"subtype = :subtype, exact_wpn = :exact, desc = :desc "
						"where mod_name = :name") )
		qDebug() << "Error prepare " << query.lastError().text();

	query.bindValue( ":name", m.name );
	query.bindValue( ":stat1", m.stat1 );
	query.bindValue( ":val1", m.val1 );
	query.bindValue( ":stat2", m.stat2 );
	query.bindValue( ":val2", m.val2 );
	query.bindValue( ":stat3", m.stat3 );
	query.bindValue( ":val3", m.val3 );
	query.bindValue( ":stat4", m.stat4 );
	query.bindValue( ":val4", m.val4 );
	query.bindValue( ":type", m.type );
	query.bindValue( ":subtype", m.subtype );
	query.bindValue( ":exact", m.exact_wpn );
	query.bindValue( ":desc", m.desc );

	return query.exec();
}

bool MainWindow::delete_mod( const QString &name )
{
	QSqlQuery query( db );

	if( !query.prepare( "delete from mods where mod_name = :name" ) )
		qDebug() << "Can't prepare " << query.lastError().text();

	query.bindValue( ":name", name );

	return query.exec();
}

void MainWindow::wpn_calc_melee( const weapon &src, weapon &dst, const double *buff )
{
	double dmg_buff = ( 1 + buff[melee_damage] ),
		   dmg_base = ( src.impact + src.puncture + src.slash + src.elemental ) * dmg_buff;

	dst.impact = src.impact * dmg_buff * ( 1 + buff[melee_impact] );
	dst.puncture = src.puncture * dmg_buff * ( 1 + buff[melee_puncture] );
	dst.slash = src.slash * dmg_buff * ( 1 + buff[melee_slash] );
	dst.elemental = src.elemental * dmg_buff;

	for( int i = melee_cold; i <= melee_toxin; ++i )
		dst.elemental += dmg_base * buff[i];

	double vs_faction = std::max( { buff[melee_corpus], buff[melee_corrupted], buff[melee_grineer] } ),
		   dmg = ( dst.impact + dst.puncture + dst.slash + dst.elemental ) * ( 1 + vs_faction );

	dst.crit = src.crit * ( 1 + buff[melee_crit] );
	dst.mult = src.mult * ( 1 + buff[melee_mult] );
	dst.rof = src.rof * ( 1 + buff[melee_rof] );
	dst.proc_x10 = dst.proc = std::min( 1.0, src.proc * ( 1 + buff[melee_proc] ) );

	dst.dps_1 = dmg * ( 1 + dst.crit * ( dst.mult - 1 ) );
	dst.dps_x10 = dst.dps_burst = dst.rof * dst.dps_1;
	dst.dps_x15 = 1.5 * dst.rof * dmg * ( 1 + src.crit * ( dst.mult - 1 ) * ( 1 + buff[melee_crit] + buff[melee_combocrit] * 1.5 ) );
	dst.dps_x20 = 2.0 * dst.rof * dmg * ( 1 + src.crit * ( dst.mult - 1 ) * ( 1 + buff[melee_crit] + buff[melee_combocrit] * 2.0 ) );
	dst.dps_x25 = 2.5 * dst.rof * dmg * ( 1 + src.crit * ( dst.mult - 1 ) * ( 1 + buff[melee_crit] + buff[melee_combocrit] * 2.5 ) );
	dst.dps_x30 = 3.0 * dst.rof * dmg * ( 1 + src.crit * ( dst.mult - 1 ) * ( 1 + buff[melee_crit] + buff[melee_combocrit] * 3.0 ) );
	dst.proc_x15 = std::min( 1.0, src.proc * ( 1 + buff[melee_proc] + buff[melee_comboproc] * 1.5 ) );
	dst.proc_x20 = std::min( 1.0, src.proc * ( 1 + buff[melee_proc] + buff[melee_comboproc] * 2.0 ) );
	dst.proc_x25 = std::min( 1.0, src.proc * ( 1 + buff[melee_proc] + buff[melee_comboproc] * 2.5 ) );
	dst.proc_x30 = std::min( 1.0, src.proc * ( 1 + buff[melee_proc] + buff[melee_comboproc] * 3.0 ) );

}

void MainWindow::wpn_calc_firing( const weapon &src, weapon &dst, const double *buff )
{
	dst.crit = src.crit * ( 1 + buff[fire_crit] );
	dst.mult = src.mult * ( 1 + buff[fire_mult] );
	dst.magazine = int( src.magazine * ( 1 + buff[fire_magazine] ) );
	dst.reload = ( src.reload + dst.magazine * src.regen ) / ( 1 + buff[fire_reload] );
	dst.ammo = int( src.ammo * ( 1 + buff[fire_ammo] ) );
	dst.rnd = src.rnd;
	dst.rof = src.rof * ( 1 + buff[fire_rof] * ( 1 + ( src.type == wpn_rifle && src.subtype == wpn_bow ) ) );
	dst.pellets = src.pellets * ( 1 + buff[fire_multishot] );
	dst.proc = std::min( 1.0, 1.0 - pow( ( 1.0 - src.proc * ( 1 + buff[fire_proc] ) ), 1 + buff[fire_multishot] ) );

	double t = dst.reload + dst.magazine / dst.rof, dmg_buff = ( 1 + buff[fire_damage] ) * ( 1 + buff[fire_multishot] );

	dst.impact = src.impact * dmg_buff;
	dst.puncture = src.puncture * dmg_buff;
	dst.slash = src.slash * dmg_buff;
	dst.elemental = src.elemental * dmg_buff;

	double dmg_base = ( dst.impact + dst.puncture + dst.slash + dst.elemental );

	dst.impact *= 1 + buff[fire_impact];
	dst.puncture *= 1 + buff[fire_puncture];
	dst.slash *= 1 + buff[fire_slash];

	for( int i = fire_cold; i <= fire_toxin; ++i )
		dst.elemental += dmg_base * buff[i];

	double vs_faction = std::max( { buff[fire_corpus], buff[fire_corrupted], buff[fire_grineer], buff[fire_infested] } ),
			dmg = ( dst.impact + dst.puncture + dst.slash + dst.elemental ) * ( 1 + vs_faction );

	dst.dps_1 = dmg * ( 1 + dst.crit * ( dst.mult - 1 ) );
	dst.dps_burst = dst.dps_1 * dst.rof;
	dst.dps_sust = dst.dps_1 * dst.magazine / t;
}

void MainWindow::wpn_calc( const weapon &src, weapon &dst, const double *buff )
{
	dst.type = src.type;
	dst.subtype = src.subtype;

	if( src.type == wpn_melee || src.type == wpn_archmelee )
		wpn_calc_melee( src, dst, buff );
	else
		wpn_calc_firing( src, dst, buff );
}

template <typename tmember>
void MainWindow::wpn_calc_dps( const weapon &src, weapon &dst, const double *buff, const QVector<mod> &list,
							   int mods_count, uint64_t &mask, tmember member, bool recursive )
{
	double b1[fire_count], b2[fire_count], shot = 0, s;
	int rec_idx = -1;

	memcpy( b1, buff, fire_count * sizeof( double ) );

	for( int i = 0; i < mods_count; ++i ) {
		int idx = -1;

		// Try all unused mods in list and find one giving best damage boost
		for( int j = 0; j < list.size(); ++j ) {
			if( ( mask & ( lull << j ) ) == 0 ) {
				memcpy( b2, b1, fire_count * sizeof( double ) );
				const auto &m = list[j];
				b2[m.stat1] += m.val1;
				b2[m.stat2] += m.val2;
				b2[m.stat3] += m.val3;
				b2[m.stat4] += m.val4;

				if( recursive && ( i < ( mods_count - 1 ) ) ) {
					// Find best pair for currently selected mod...
					double b3[fire_count];
					uint64_t mask3 = mask | ( lull << j );
					s = shot;

					for( int q = 0; q < list.size(); ++q )
						if( ( mask3 & ( lull << q ) ) == 0 ) {
							memcpy( b3, b2, fire_count * sizeof( double ) );
							const auto &m3 = list[q];
							b3[m3.stat1] += m3.val1;
							b3[m3.stat2] += m3.val2;
							b3[m3.stat3] += m3.val3;
							b3[m3.stat4] += m3.val4;
							wpn_calc( src, dst, b3 );

							if( &dst->*member > s ) {
								s = &dst->*member;
								rec_idx = q;
							}
						}
				} else {
					wpn_calc( src, dst, b2 );
					s = &dst->*member;
				}

				if( s > shot ) {
					idx = j;
					shot = s;
				}
			}
		}

		if( idx >= 0 ) {
			// We found one best mod
			const mod &m = list[idx];
			mask |= lull << idx;
			b1[m.stat1] += m.val1;
			b1[m.stat2] += m.val2;
			b1[m.stat3] += m.val3;
			b1[m.stat4] += m.val4;
		} else if( recursive && rec_idx >= 0 ) {
			// Add last recursed mod
			const mod &m = list[rec_idx];
			mask |= lull << rec_idx;
			b1[m.stat1] += m.val1;
			b1[m.stat2] += m.val2;
			b1[m.stat3] += m.val3;
			b1[m.stat4] += m.val4;
		} else
			break;	// No chances to find more good mods
	}

	wpn_calc( src, dst, b1 );
}

void MainWindow::wpn_build(const weapon &src, const QVector<mod> &forced, const QVector<mod> &list, int n, int riven __attribute__((unused)),
							bool neg __attribute__((unused)), weapon &dst_1, weapon &dst_burst, weapon &dst_sust, MainWindow *self )
{
	if( list.size() > 64 ) {
		qDebug() << "Too many mods";	// Can be done other way.
		return;
	}

	double buff[fire_count] = {0};	// fire_count == melee_count
	uint64_t best_shot = 0, best_burst = 0, best_sust = 0;

	for( const auto &f : forced ) {
		buff[f.stat1] += f.val1;
		buff[f.stat2] += f.val2;
		buff[f.stat3] += f.val3;
		buff[f.stat4] += f.val4;
	}

	if( list.size() == 0 || forced.size() >= n ) {
		wpn_calc( src, dst_1, buff );
		dst_burst = dst_sust = dst_1;
		return;
	}

	if( src.type == wpn_melee || src.type == wpn_archmelee ) {
		wpn_calc_dps( src, dst_1, buff, list, n - forced.size(), best_shot, &weapon::dps_x10, self->ui->calc_recursion->isChecked() );
		wpn_calc_dps( src, dst_burst, buff, list, n - forced.size(), best_burst, &weapon::dps_x20, self->ui->calc_recursion->isChecked() );
		wpn_calc_dps( src, dst_sust, buff, list, n - forced.size(), best_sust, &weapon::dps_x30, self->ui->calc_recursion->isChecked() );
	} else {
		wpn_calc_dps( src, dst_1, buff, list, n - forced.size(), best_shot, &weapon::dps_1, self->ui->calc_recursion->isChecked() );
		wpn_calc_dps( src, dst_burst, buff, list, n - forced.size(), best_burst, &weapon::dps_burst, self->ui->calc_recursion->isChecked() );
		wpn_calc_dps( src, dst_sust, buff, list, n - forced.size(), best_sust, &weapon::dps_sust, self->ui->calc_recursion->isChecked() );
	}

	for( int i = 0; i < list.size(); ++i )
		if( best_shot & ( lull << i ) )
			mods_1.push_back( list[i] );

	for( int i = 0; i < list.size(); ++i )
		if( best_burst & ( lull << i ) )
			mods_burst.push_back( list[i] );

	for( int i = 0; i < list.size(); ++i )
		if( best_sust & ( lull << i ) )
			mods_sust.push_back( list[i] );
}

void MainWindow::wpn_riven(const weapon &src, const QVector<mod> &forced, const QVector<mod> &list, int n, int riven, bool neg,
						   weapon &dst_1, weapon &dst_burst, weapon &dst_sust, MainWindow *self )
{
	if( list.size() > 64 ) {
		qDebug() << "Too many mods";	// Can be done other way.
		return;
	}

	double buff[fire_count] = {0};	// fire_count > melee_count
	uint64_t best_shot = 0, best_burst = 0, best_sust = 0;

	for( const auto &f : forced ) {
		buff[f.stat1] += f.val1;
		buff[f.stat2] += f.val2;
		buff[f.stat3] += f.val3;
		buff[f.stat4] += f.val4;
	}

	if( list.size() == 0 || forced.size() >= n ) {
		wpn_calc( src, dst_1, buff );
		dst_burst = dst_sust = dst_1;
		return;
	}

	if( src.type == wpn_melee || src.type == wpn_archmelee ) {
		wpn_calc_riven( src, dst_1, buff, list, n - forced.size() - 1, riven, best_shot, &weapon::dps_x10, rm_1, mods_1, neg, self );
		wpn_calc_riven( src, dst_burst, buff, list, n - forced.size() - 1, riven, best_burst, &weapon::dps_x20, rm_burst, mods_burst, neg, self );
		wpn_calc_riven( src, dst_sust, buff, list, n - forced.size() - 1, riven, best_sust, &weapon::dps_x30, rm_sust, mods_sust, neg, self );
	} else {
		wpn_calc_riven( src, dst_1, buff, list, n - forced.size() - 1, riven, best_shot, &weapon::dps_1, rm_1, mods_1, neg, self );
		wpn_calc_riven( src, dst_burst, buff, list, n - forced.size() - 1, riven, best_burst, &weapon::dps_burst, rm_burst, mods_burst, neg, self );
		wpn_calc_riven( src, dst_sust, buff, list, n - forced.size() - 1, riven, best_sust, &weapon::dps_sust, rm_sust, mods_sust, neg, self );
	}

	self->draw_mod_list( self->ui->calc_mods_riven_1, rm_1 );
	self->draw_mod_list( self->ui->calc_mods_riven_burst, rm_burst );
	self->draw_mod_list( self->ui->calc_mods_riven_sust, rm_sust );
}

template <typename tmember>
void MainWindow::wpn_calc_riven( const weapon &src, weapon &dst, const double *buff, const QVector<mod> &list, int mods_count, int riven,
								 uint64_t &out_mask, tmember member, QVector<mod> &r_stat, QVector<mod> &mods, bool neg, MainWindow *self )
{
	// Try all possible riven 2 or 3 stats combinations and optimize build for each
	bool is_melee = ( src.type == wpn_melee || src.type == wpn_archmelee );
	const QStringList &stat_names = is_melee ? stats_melee : stats_firing;
	double b0[fire_count + 1], r0[fire_count + 1], *b1 = b0 + 1, *rv_table = r0 + 1, adjust = src.dispo * ( neg ? 1.25 : 1.0 ), shot = 0.0;
	int last = 0, third0 = fire_none, third1 = third0 + 1, stats[fire_count];
	uint64_t mask = 0, best_mask = 0, best_rv = 0;
	weapon best_wpn;

	// Build list of allowed riven buffs
	for( int i = 0, e = is_melee ? melee_count : fire_count; i < e; ++i ) {
		if( self->ui->calc_riven_no_phy->isChecked() && i >= stat_impact  && i <= stat_slash )
			continue;

		if( self->ui->calc_riven_no_ele->isChecked() && i >= stat_cold && i <= stat_toxin )
			continue;

		if( self->ui->calc_riven_no_fact->isChecked() && i >= stat_corpus && i <= stat_infested )
			continue;

		stats[last++] = i;
	}

	// Adjust stats for two-buffs riven
	if( riven == 3 ) {
		third0 = 0;
		third1 = last;
	} else
		adjust *= 1.32;

	adjust *= 1.11;
	self->ui->calc_riven_progress->setMaximum( ( 2 + ( src.type != wpn_melee && src.type != wpn_archmelee ) ) * ( third1 - third0 ) * last - 1 );

	// Select right buffs table and put in all the modifiers
	if( src.type == wpn_rifle )
		for( int i = 0; i < fire_count; ++i )
			rv_table[i] = riven_table_rifle[i] * adjust;
	else if( src.type == wpn_shotgun )
		for( int i = 0; i < fire_count; ++i )
			rv_table[i] = riven_table_shotgun[i] * adjust;
	else if( src.type == wpn_secondary )
		for( int i = 0; i < fire_count; ++i )
			rv_table[i] = riven_table_secondary[i] * adjust;
	else if( src.type == wpn_melee )
		for( int i = 0; i < melee_count; ++i )
			rv_table[i] = riven_table_melee[i] * adjust;

	// Do try out all the possible combinations and find which one is best.
	for( int a = third0; a < third1; ++a )
		for( int b = 0; b < last; ++b ) {
			self->ui->calc_riven_progress->setValue( self->ui->calc_riven_progress->value() + 1 );

			for( int c = 0; c < last; ++c )
				if( a != b && a != c && b != c ) {
					memcpy( b1, buff, fire_count * sizeof( double ) );
					b1[stats[a]] += rv_table[stats[a]];
					b1[stats[b]] += rv_table[stats[b]];
					b1[stats[c]] += rv_table[stats[c]];
					mask = 0;
					wpn_calc_dps( src, dst, b1, list, mods_count, mask, member, false );

					if( &dst->*member > shot ) {
						shot = &dst->*member;
						best_mask = mask;
						best_rv = ( lull << a ) | ( lull << b ) | ( lull << c );
						best_wpn = dst;
					}
				}
		}

	for( int i = 0; i < list.size(); ++i )
		if( best_mask & ( lull << i ) )
			mods << list[i];

	r_stat.clear();

	for( int i = 0; i < last; ++i )
		if( best_rv & ( lull << i ) ) {
			mod m;

			m.stat1 = stats[i];
			m.val1 = rv_table[stats[i]];
			m.type = src.type;
			m.subtype = src.subtype;
			m.exact_wpn = src.name;
			m.desc = "Recommended riven mod buff";
			m.name = stat_names.at( stats[i] );

			r_stat << m;
		}

	out_mask = best_mask;
	dst = best_wpn;
}

double MainWindow::parse_percent( const QString &str )
{
	if( str.endsWith( '%' ) )
		return str.left( str.size() - 1 ).toDouble() / 100.0;
	else
		return str.toDouble();
}


void MainWindow::on_calc_riven2_clicked()
{
	on_calc_riven_clicked( 2 );
}

void MainWindow::on_calc_riven3_clicked()
{
	on_calc_riven_clicked( 3 );
}

void MainWindow::draw_mod_list( QListWidget *wgt, QVector<mod> &mods )
{
	wgt->clear();

	for( const auto &m : mods )
		wgt->addItem( m.name );
}

void MainWindow::draw_mod( const mod &m )
{
	const QStringList &slist = ( m.type == wpn_melee || m.type == wpn_archmelee ) ? stats_melee : stats_firing;

	ui->calc_mod_name->setText( m.name );
	ui->calc_mod_desc->setText( m.desc );
	ui->calc_mod_stat1->setText( slist.at( m.stat1 ) );
	ui->calc_mod_val1->setText( printf( m.val1 ) );

	if( m.stat2 >= 0 ) {
		ui->calc_mod_stat2->setText( slist.at( m.stat2 ) );
		ui->calc_mod_val2->setText( printf( m.val2 ) );
	} else {
		ui->calc_mod_stat2->clear();
		ui->calc_mod_val2->clear();
	}

	if( m.stat3 >= 0 ) {
		ui->calc_mod_stat3->setText( slist.at( m.stat3 ) );
		ui->calc_mod_val3->setText( printf( m.val3 ) );
	} else {
		ui->calc_mod_stat3->clear();
		ui->calc_mod_val3->clear();
	}

	if( m.stat4 >= 0 ) {
		ui->calc_mod_stat4->setText( slist.at( m.stat4 ) );
		ui->calc_mod_val4->setText( printf( m.val4 ) );
	} else {
		ui->calc_mod_stat4->clear();
		ui->calc_mod_val4->clear();
	}
}

void MainWindow::on_calc_mods_riven_1_itemClicked(QListWidgetItem *item)
{
	for( const auto &m : rm_1 )
		if( item->text() == m.name )
			draw_mod( m );
}


void MainWindow::on_calc_mods_riven_burst_itemClicked(QListWidgetItem *item)
{
	for( const auto &m : rm_burst )
		if( item->text() == m.name )
			draw_mod( m );

}

void MainWindow::on_calc_mods_riven_sust_itemClicked(QListWidgetItem *item)
{
	for( const auto &m : rm_sust )
		if( item->text() == m.name )
			draw_mod( m );

}

void MainWindow::on_calc_grineer_toggled(bool checked)
{
	if( !checked ) {
		ui->calc_grineer_cloned->setChecked( checked );
		ui->calc_grineer_ferrite->setChecked( checked );
		ui->calc_grineer_alloy->setChecked( checked );
		ui->calc_grineer_machinery->setChecked( checked );
	}

	ui->calc_grineer_cloned->setDisabled( !checked );
	ui->calc_grineer_ferrite->setDisabled( !checked );
	ui->calc_grineer_alloy->setDisabled( !checked );
	ui->calc_grineer_machinery->setDisabled( !checked );
}

void MainWindow::on_calc_grineer_cloned_toggled(bool checked)
{
	ui->calc_grineer_cloned_val->setDisabled( !checked );
	balance_damage();
}

void MainWindow::on_calc_grineer_ferrite_toggled(bool checked)
{
	ui->calc_grineer_ferrite_val->setDisabled( !checked );
	balance_damage();
}

void MainWindow::on_calc_grineer_alloy_toggled(bool checked)
{
	ui->calc_grineer_alloy_val->setDisabled( !checked );
	balance_damage();
}

void MainWindow::on_calc_grineer_machinery_toggled(bool checked)
{
	ui->calc_grineer_machinery_val->setDisabled( !checked );
	balance_damage();
}


void MainWindow::on_calc_corpus_toggled(bool checked)
{
	if( !checked ) {
		ui->calc_corpus_flesh->setChecked( checked );
		ui->calc_corpus_shield->setChecked( checked );
		ui->calc_corpus_proto->setChecked( checked );
		ui->calc_corpus_robotic->setChecked( checked );
	}

	ui->calc_corpus_flesh->setDisabled( !checked );
	ui->calc_corpus_shield->setDisabled( !checked );
	ui->calc_corpus_proto->setDisabled( !checked );
	ui->calc_corpus_robotic->setDisabled( !checked );
}

void MainWindow::on_calc_corpus_flesh_toggled(bool checked)
{
	ui->calc_corpus_flesh_val->setDisabled( !checked );
	balance_damage();
}

void MainWindow::on_calc_corpus_shield_toggled(bool checked)
{
	ui->calc_corpus_shield_val->setDisabled( !checked );
	balance_damage();
}

void MainWindow::on_calc_corpus_proto_toggled(bool checked)
{
	ui->calc_corpus_proto_val->setDisabled( !checked );
	balance_damage();
}

void MainWindow::on_calc_corpus_robotic_toggled(bool checked)
{
	ui->calc_corpus_robotic_val->setDisabled( !checked );
	balance_damage();
}

void MainWindow::on_calc_infested_toggled(bool checked)
{
	if( !checked ) {
		ui->calc_infested_infested->setChecked( checked );
		ui->calc_infested_flesh->setChecked( checked );
		ui->calc_infested_fossil->setChecked( checked );
		ui->calc_infested_sinew->setChecked( checked );
	}

	ui->calc_infested_infested->setDisabled( !checked );
	ui->calc_infested_flesh->setDisabled( !checked );
	ui->calc_infested_fossil->setDisabled( !checked );
	ui->calc_infested_sinew->setDisabled( !checked );
}

void MainWindow::on_calc_infested_infested_toggled(bool checked)
{
	ui->calc_infested_infested_val->setDisabled( !checked );
	balance_damage();
}

void MainWindow::on_calc_infested_flesh_toggled(bool checked)
{
	ui->calc_infested_flesh_val->setDisabled( !checked );
	balance_damage();
}

void MainWindow::on_calc_infested_fossil_toggled(bool checked)
{
	ui->calc_infested_fossil_val->setDisabled( !checked );
	balance_damage();
}

void MainWindow::on_calc_infested_sinew_toggled(bool checked)
{
	ui->calc_infested_sinew_val->setDisabled( !checked );
	balance_damage();
}

void MainWindow::balance_damage( void )
{
	int sum = 0, count = 0;
	double adjust = 1.0, half = 0.0;

	for( int i = 0; i < 10; ++i )
		if( damage[i]->isEnabled() ) {
			sum += damage[i]->value();
			++count;
		}

	qDebug() << sum << count;

	if( count == 0 )
		return;

	if( sum == 0 )
		sum = count;

	if( sum < 100 )
		half = ( 100.0 - sum ) / count;
	else
		adjust = 100.0 / sum;

	qDebug() << sum << count << half;

	if( abs( sum - 100 ) > count )
		for( int i = 0; i < 12; ++i )
			if( damage[i]->isEnabled() )
				damage[i]->setValue( adjust * damage[i]->value() + half );
}

void MainWindow::set_combo_weapons( QComboBox *cb, const QVector<weapon> &wl )
{
	cb->clear();

	for( auto it = wl.begin(); it < wl.end(); ++it )
		cb->addItem( it->name, it );
}

void MainWindow::set_combo_mods( QComboBox *cb, const QVector<mod> &ml )
{
	cb->clear();

	for( auto it = ml.begin(); it < ml.end(); ++it )
		cb->addItem( it->name, it );
}
