/*
Copyright (c) 2011, Andre Somers
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the Rathenau Instituut, Andre Somers nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL ANDRE SOMERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR #######; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

/*
 * ToDo:
 *  - qsrand -> QRandomGenerator
 *
 * */


#include "SimpleCryptQt.h"
#include <QByteArray>
#include <QtDebug>
#include <QtGlobal>
#include <QDateTime>
#include <QCryptographicHash>
#include <QDataStream>


// ********************************************************************
SimpleCryptQt::SimpleCryptQt():
	mu8Key(0),
	meCompressionMode(CompressionMode_e::Auto),
	meProtectionMode(IntegrityProtectionMode_e::Checksum),
	meLastError(Error_e::NoError)
{
	qsrand(uint(QDateTime::currentMSecsSinceEpoch() & 0xFFFF));
}


// ********************************************************************
SimpleCryptQt::SimpleCryptQt(quint64 au8Key):
	mu8Key(au8Key),
	meCompressionMode(CompressionMode_e::Auto),
	meProtectionMode(IntegrityProtectionMode_e::Checksum),
	meLastError(Error_e::NoError)
{
	qsrand(uint(QDateTime::currentMSecsSinceEpoch() & 0xFFFF));
	mSplitKey();
}


// ********************************************************************
void
SimpleCryptQt::mSetKey(quint64 au8Key)
{
	mu8Key = au8Key;
	mSplitKey();
}


// ********************************************************************
void
SimpleCryptQt::mSplitKey()
{
	muvKeyParts.clear();
	muvKeyParts.resize(8);
	for (int i=0;i<8;i++) {
		quint64 u8Part = mu8Key;
		for (int j=i; j>0; j--) {
			u8Part = u8Part >> 8;
		}
		u8Part = u8Part & 0xff;
		muvKeyParts[i] = static_cast<char>(u8Part);
	}
}


// ********************************************************************
QByteArray
SimpleCryptQt::mEncryptToByteArray(const QString& asrPlaintext)
{
	QByteArray sPlaintextArray = asrPlaintext.toUtf8();
	return mEncryptToByteArray(sPlaintextArray);
}


// ********************************************************************
QByteArray
SimpleCryptQt::mEncryptToByteArray(QByteArray asPlaintext)
{
	if (muvKeyParts.isEmpty()) {
		qWarning() << "No key set.";
		meLastError = Error_e::NoKeySet;
		return QByteArray();
	}

	QByteArray sBa = asPlaintext;

	CryptoFlags eFlags = CryptoFlag_e::None;
	if (meCompressionMode == CompressionMode_e::Always) {
		sBa = qCompress(sBa, 9); //maximum compression
		eFlags |= CryptoFlag_e::Compression;
	} else if (meCompressionMode == CompressionMode_e::Auto) {
		QByteArray compressed = qCompress(sBa, 9);
		if (compressed.count() < sBa.count()) {
			sBa = compressed;
			eFlags |= CryptoFlag_e::Compression;
		}
	}

	QByteArray sIntegrityProtection;
	if (meProtectionMode == IntegrityProtectionMode_e::Checksum) {
		eFlags |= CryptoFlag_e::Checksum;
		QDataStream s(&sIntegrityProtection, QIODevice::WriteOnly);
		s << qChecksum(sBa.constData(), sBa.length());
	} else if (meProtectionMode == IntegrityProtectionMode_e::Hash) {
		eFlags |= CryptoFlag_e::Hash;
		QCryptographicHash hash(QCryptographicHash::Sha1);
		hash.addData(sBa);

		sIntegrityProtection += hash.result();
	}

	//prepend a random char to the string
	char  cRandomChar = char(qrand() & 0xFF);
	sBa = cRandomChar + sIntegrityProtection + sBa;

	int  iPos(0);
	char cLastChar(0);

	int  iCnt = sBa.count();

	while (iPos <  iCnt) {
		sBa[iPos] = sBa.at(iPos) ^ muvKeyParts.at(iPos % 8) ^ cLastChar;
		cLastChar = sBa.at(iPos);
		++iPos;
	}

	QByteArray sResultArray;
	sResultArray.append(char(0x03));  //version for future updates to algorithm
	sResultArray.append(char(eFlags)); //encryption flags
	sResultArray.append(sBa);

	meLastError = Error_e::NoError;
	return sResultArray;
}


// ********************************************************************
QString
SimpleCryptQt::msEncryptToString(const QString& asrPlaintext)
{
	QByteArray asPlaintextArray = asrPlaintext.toUtf8();
	QByteArray asCypher = mEncryptToByteArray(asPlaintextArray);
	QString asCypherString = QString::fromLatin1(asCypher.toBase64());
	return asCypherString;
}


// ********************************************************************
QString
SimpleCryptQt::msEncryptToString(QByteArray asPlaintext)
{
	QByteArray sCypher = mEncryptToByteArray(asPlaintext);
	QString sCypherString = QString::fromLatin1(sCypher.toBase64());
	return sCypherString;
}


// ********************************************************************
QString
SimpleCryptQt::msDecryptToString(const QString& asrCyphertext)
{
	QByteArray sCyphertextArray = QByteArray::fromBase64(asrCyphertext.toLatin1());
	QByteArray sPlaintextArray = msDecryptToByteArray(sCyphertextArray);
	QString sPlaintext = QString::fromUtf8(sPlaintextArray, sPlaintextArray.size());

	return sPlaintext;
}


// ********************************************************************
QString
SimpleCryptQt::msDecryptToString(QByteArray asCypher)
{
	QByteArray ba = msDecryptToByteArray(asCypher);
	QString sPlaintext = QString::fromUtf8(ba, ba.size());

	return sPlaintext;
}


// ********************************************************************
QByteArray
SimpleCryptQt::msDecryptToByteArray(const QString& msrCypherText)
{
	QByteArray sCyphertextArray = QByteArray::fromBase64(msrCypherText.toLatin1());
	QByteArray ba = msDecryptToByteArray(sCyphertextArray);

	return ba;
}


// ********************************************************************
QByteArray
SimpleCryptQt::msDecryptToByteArray(QByteArray asCypher)
{
	if (muvKeyParts.isEmpty()) {
		qWarning() << "No key set.";
		meLastError = Error_e::NoKeySet;
		return QByteArray();
	}

	QByteArray ba = asCypher;

	if( asCypher.count() < 3 ) {
		return QByteArray();
	}

	char cVersion = ba.at(0);

	if (cVersion !=3) {  //we only work with version 3
		meLastError = Error_e::UnknownVersion;
		qWarning() << "Invalid version or not a cyphertext.";
		return QByteArray();
	}

	CryptoFlags eFlags = CryptoFlags(ba.at(1));

	ba = ba.mid(2);
	int  iPos(0);
	int  iCnt(ba.count());
	char cLastChar = 0;

	while (iPos <  iCnt) {
		char currentChar = ba[iPos];
		ba[iPos] = ba.at(iPos) ^ cLastChar ^ muvKeyParts.at(iPos % 8);
		cLastChar = currentChar;
		++iPos;
	}

	ba = ba.mid(1); //chop off the random number at the start

	bool bIntegrityOk(true);
	if (eFlags.testFlag(CryptoFlag_e::Checksum)) {
		if (ba.length() < 2) {
			meLastError = Error_e::IntegrityFailed;
			return QByteArray();
		}
		quint16 uStoredChecksum;
		{
			QDataStream s(&ba, QIODevice::ReadOnly);
			s >> uStoredChecksum;
		}
		ba = ba.mid(2);
		quint16 uChecksum = qChecksum(ba.constData(), ba.length());
		bIntegrityOk = (uChecksum == uStoredChecksum);

	} else if (eFlags.testFlag(CryptoFlag_e::Hash)) {
		if (ba.length() < 20) {
			meLastError = Error_e::IntegrityFailed;
			return QByteArray();
		}
		QByteArray storedHash = ba.left(20);
		ba = ba.mid(20);
		QCryptographicHash oHash(QCryptographicHash::Sha1);
		oHash.addData(ba);
		bIntegrityOk = (oHash.result() == storedHash);
	}

	if (!bIntegrityOk) {
		meLastError = Error_e::IntegrityFailed;
		return QByteArray();
	}

	if (eFlags.testFlag(CryptoFlag_e::Compression))
		ba = qUncompress(ba);

	meLastError = Error_e::NoError;
	return ba;
}
