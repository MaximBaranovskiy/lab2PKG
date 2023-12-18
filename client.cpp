#include "client.h"
#include "ui_client.h"
#include <QFileDialog>
#include <QImage>
#include<QImageWriter>

Client::Client(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Client)
{
    ui->setupUi(this);

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

}

Client::~Client()
{
    delete ui;
}

void Client::getImageInfo(const QString path)
{
    QImage img(path);
    QFileInfo fileInfo(path);

    QImageWriter a(path);
    int compression = a.compression();

    QString nameOfTheFile = fileInfo.fileName();
    QString imageSizeString = QString("%1 x %2").arg(img.width()).arg(img.height());
    QString imageResolution = QString("%1 x %2").arg((int)(img.dotsPerMeterX() * 0.0254)).arg((int)(img.dotsPerMeterY() * 0.0254));
    int colorDepth = img.bitPlaneCount();

    QTableWidgetItem* name = new QTableWidgetItem(nameOfTheFile);
    QTableWidgetItem* size = new QTableWidgetItem(imageSizeString);
    QTableWidgetItem* resolution= new QTableWidgetItem(imageResolution);
    QTableWidgetItem* depth = new QTableWidgetItem(QString::number(colorDepth));
    QTableWidgetItem* com= new QTableWidgetItem(QString::number(compression));

    name->setTextAlignment(Qt::AlignCenter);
    size->setTextAlignment(Qt::AlignCenter);
    resolution->setTextAlignment(Qt::AlignCenter);
    depth->setTextAlignment(Qt::AlignCenter);

    int row = ui->tableWidget->rowCount();
    ui->tableWidget->setRowCount(row + 1);
    ui->tableWidget->setItem(row, 0, name);
    ui->tableWidget->setItem(row, 1, size);
    ui->tableWidget->setItem(row, 2, resolution);
    ui->tableWidget->setItem(row, 3, depth);
    ui->tableWidget->setItem(row, 4, com);

}

bool Client::isImageValid(const QString imagePath)
{
        QStringList allowedExtensions = {"jpg", "jpeg", "png", "bmp", "gif", "tiff", "pcx"};
        QFileInfo fileInfo(imagePath);
        if (fileInfo.exists() && fileInfo.isFile()) {
            QString extension = fileInfo.suffix().toLower();
            return allowedExtensions.contains(extension);
        }
        return false;

}

QString Client::getImageFormat(const QString imagePath)
{
        QFileInfo fileInfo(imagePath);
        return fileInfo.suffix().toLower();
}

void Client::on_pushButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, "Открыть файл", "", "Все файлы (*.*);;Текстовые файлы (*.txt)");

    if (!fileName.isEmpty()) {

        qDebug() << "Выбран файл: " << fileName;
        if(isImageValid(fileName)){
            getImageInfo(fileName);
        }
    }
}


void Client::on_pushButton_3_clicked()
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
}


void Client::on_pushButton_2_clicked()
{
    QString directoryPath = QFileDialog::getExistingDirectory(this, "Выбрать папку", "", QFileDialog::ShowDirsOnly);

        if (!directoryPath.isEmpty()) {
        QDir directory(directoryPath);
        QStringList filters = {"*.jpg", "*.jpeg", "*.png", "*.bmp", "*.gif", "*.tiff", "*.pcx"};
        QStringList files = directory.entryList(filters, QDir::Files);

        foreach (const QString &file, files) {
            QString filePath = directory.filePath(file);
            if (isImageValid(filePath)) {
                getImageInfo(filePath);
            }
        }
        }
}

