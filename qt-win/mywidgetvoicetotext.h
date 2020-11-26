#ifndef MYWIDGETVOICETOTEXT_H
#define MYWIDGETVOICETOTEXT_H

#include <QMainWindow>
#include <QTextEdit>
#include <QPushButton>
#include <QFile>
#include <QDir>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QHBoxLayout>

#include <QSplitter>
#include <QLineEdit>
#include <QFrame>
#include <QSpacerItem>
#include <QMessageBox>




class myWidgetVoiceToText : public QMainWindow
{
    Q_OBJECT

    enum class chusePathVariants
    {
       CHUSEPATHFILE,
       CHUSEPATHDIR
    };

public:
    myWidgetVoiceToText(QWidget *parent = nullptr);
    ~myWidgetVoiceToText();


    QString passFile() const;
    void setPassFile(const QString &passFile);

    QString passModel() const;
    void setPassModel(const QString &passModel);

    QString convertWavToString(QByteArray massFile,QString passModel);




private:
    QString m_passFile;
    QString m_passModel;
    QByteArray m_fileMass;

    QPushButton * decode;
    QPushButton * openDialogChusePassModelDir;
    QPushButton * openDialogChusePassFile;

    QLineEdit   * linePassFile;
    QLineEdit   * linePassModel;
    QTextEdit   * visualizationRezult;

private slots:

    void createUI();
    void showMessage(QString message);

    inline void  choicedir()
    {

       QString dir = QFileDialog::getExistingDirectory(0, ("chuse catal model"),
                                                 QDir::rootPath(),
                                                 QFileDialog::ShowDirsOnly
                                                 | QFileDialog::DontResolveSymlinks);

       linePassModel->setText(dir);
       setPassFile(dir);
    }
    inline void choicefile()
    {
        QString dir;

        dir = QFileDialog::getOpenFileName(0, tr("chuse file"),
                                            "",
                                            ("file (*.wav)"));

        linePassFile->setText(dir);
        setPassModel(dir);

    }

    QByteArray readFile(QString file);



};
#endif // MYWIDGETVOICETOTEXT_H
