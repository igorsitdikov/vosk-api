#include "mywidgetvoicetotext.h"
#include <QDebug>
#include "model.h"
#include "kaldi_recognizer.h"

myWidgetVoiceToText::myWidgetVoiceToText(QWidget *parent)
    : QMainWindow(parent)
{
    resize(500,300);
    setWindowTitle("Converter WavToText");
    createUI();
}

myWidgetVoiceToText::~myWidgetVoiceToText()
{
}

void myWidgetVoiceToText::createUI()
{
    QFrame * frame = new QFrame(this);
    setCentralWidget(frame);

    QVBoxLayout * lout = new QVBoxLayout(frame);

    openDialogChusePassModelDir = new QPushButton("choose path model");
    openDialogChusePassFile     = new QPushButton("choose path file");
    decode                      = new QPushButton("decode");

    linePassModel               = new QLineEdit();
    linePassFile                = new QLineEdit();
    visualizationRezult         = new QTextEdit();


    QVBoxLayout * ButtonLout = new QVBoxLayout(frame);
    ButtonLout->addWidget(openDialogChusePassModelDir);
    ButtonLout->addWidget(openDialogChusePassFile);
    ButtonLout->addWidget(decode);


    QVBoxLayout * lineEditLout = new QVBoxLayout(frame);
    lineEditLout->addWidget(linePassModel);
    lineEditLout->addWidget(linePassFile);
    lineEditLout->addSpacing(30);

    QHBoxLayout * hlut = new QHBoxLayout(frame);
    hlut->addLayout(ButtonLout);
    hlut->addLayout(lineEditLout);

    lout->addLayout(hlut);
    lout->addWidget(visualizationRezult);

    connect(openDialogChusePassFile,&QPushButton::clicked,this,&myWidgetVoiceToText::choicefile);
    connect(openDialogChusePassModelDir,&QPushButton::clicked,this,&myWidgetVoiceToText::choicedir);
    connect(decode,&QPushButton::clicked,[this](){

        try {
            QByteArray in = readFile(linePassFile->text());
            QString out = convertWavToString(in,linePassModel->text());
            visualizationRezult->setText(out);

        } catch (QString out) {
            qDebug()<<out;
        }


    });
}

QByteArray myWidgetVoiceToText::readFile(QString filePath)
{
    QFile file(filePath);
    if(!file.open(QIODevice::ReadOnly))
    {
        throw(QString("Error file reading:"+filePath));
    }

    return file.readAll();
}

QString myWidgetVoiceToText::passFile() const
{
    return m_passFile;
}

void myWidgetVoiceToText::setPassFile(const QString &passFile)
{
    m_passFile = passFile;
}

QString myWidgetVoiceToText::passModel() const
{
    return m_passModel;
}

void myWidgetVoiceToText::setPassModel(const QString &passModel)
{
    m_passModel = passModel;
}


QString myWidgetVoiceToText::convertWavToString(QByteArray massFile, QString passModel)
{
    if(passModel.isEmpty())
        throw QString("path to model is empty");

    if(massFile.isEmpty())
        throw QString("array file is empty");

    const char * inData = massFile.data();
    Model *model = new Model(passModel.toStdString().c_str());
    KaldiRecognizer *recognizer = new KaldiRecognizer(model, 16000);
    recognizer->AcceptWaveform(inData, massFile.size());
    const char * outString=recognizer->FinalResult();
    return QString(outString);
}


void myWidgetVoiceToText::showMessage(QString message)
{
    QMessageBox msg;
    msg.setWindowTitle("Massage!");
    msg.setText(message);
    msg.exec();
}



