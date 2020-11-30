#ifndef APPOPTIONS_H
#define APPOPTIONS_H

#pragma once
#include <QDialog>
#include <cereal/access.hpp>

namespace Ui {
class AppOptions;
}

class AppOptions : public QDialog
{
    Q_OBJECT

public:
    explicit AppOptions(QWidget *parent = nullptr);
    ~AppOptions();
    bool OptionsChanged();

private:
    Ui::AppOptions *ui;

    bool options_changed_ = false;

    struct Data{
        bool respect_last_transfer_list;
    } data_;

    friend class cereal::access;
    template<class Archive>
    void save(Archive &ar) const { ar(data_); };
    template<class Archive>
    void load(Archive &ar){ ar(data_); }
};

#endif // APPOPTIONS_H
