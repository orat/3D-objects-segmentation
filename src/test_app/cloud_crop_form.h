#ifndef CLOUD_CROP_TEST_FORM_H
#define CLOUD_CROP_TEST_FORM_H

#include "test_lib.h"

#include <cfloat>

#include <QDialog>
#include <QFileDialog>
#include <QMessageBox>
#include <QErrorMessage>

namespace Ui {
class cloud_crop_test_form;
}

class cloud_crop_form : public QDialog
{
    Q_OBJECT

public:
    explicit cloud_crop_form(QWidget *parent = 0);
    ~cloud_crop_form();

private slots:
    void on_cloud_in_browse_btn_clicked();

    void on_cloud_out_browse_btn_clicked();

    void on_launch_test_btn_clicked();

    void on_cancel_btn_clicked();

    void on_help_btn_clicked();

private:
    Ui::cloud_crop_test_form *ui;
};

#endif // CLOUD_CROP_TEST_FORM_H
