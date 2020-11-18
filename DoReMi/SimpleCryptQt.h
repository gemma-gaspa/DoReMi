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

#ifndef SIMPLECRYPT_H
#define SIMPLECRYPT_H
#include <QString>
#include <QVector>
#include <QFlags>

/**
  @short Simple encryption and decryption of strings and byte arrays

  This class provides a simple implementation of encryption and decryption
  of strings and byte arrays.

  @warning The encryption provided by this class is NOT strong encryption. It may
  help to shield things from curious eyes, but it will NOT stand up to someone
  determined to break the encryption. Don't say you were not warned.

  The class uses a 64 bit key. Simply create an instance of the class, set the key,
  and use the encryptToString() method to calculate an encrypted version of the input string.
  To decrypt that string again, use an instance of SimpleCrypt initialized with
  the same key, and call the decryptToString() method with the encrypted string. If the key
  matches, the decrypted version of the string will be returned again.

  If you do not provide a key, or if something else is wrong, the encryption and
  decryption function will return an empty string or will return a string containing nonsense.
  lastError() will return a value indicating if the method was succesful, and if not, why not.

  SimpleCrypt is prepared for the case that the encryption and decryption
  algorithm is changed in a later version, by prepending a version identifier to the cypertext.
  */


class SimpleCryptQt
{
public:
	/**
	  CompressionMode describes if compression will be applied to the data to be
	  encrypted.
	  */
	enum class CompressionMode_e {
		Auto,    /*!< Only apply compression if that results in a shorter plaintext. */
		Always,  /*!< Always apply compression. Note that for short inputs, a compression may result in longer data */
		Never    /*!< Never apply compression. */
	};

	/**
	  IntegrityProtectionMode describes measures taken to make it possible to detect problems with the data
	  or wrong decryption keys.

	  Measures involve adding a checksum or a cryptograhpic hash to the data to be encrypted. This
	  increases the length of the resulting cypertext, but makes it possible to check if the plaintext
	  appears to be valid after decryption.
	*/
	enum class IntegrityProtectionMode_e {
		None,    /*!< The integerity of the encrypted data is not protected. It is not really possible to detect a wrong key, for instance. */
		Checksum,/*!< A simple checksum is used to verify that the data is in order. If not, an empty string is returned. */
		Hash     /*!< A cryptographic hash is used to verify the integrity of the data. This method produces a much stronger, but longer check */
	};

	/**
	  Error describes the type of error that occured.
	  */
	enum class Error_e {
		NoError,         /*!< No error occurred. */
		NoKeySet,        /*!< No key was set. You can not encrypt or decrypt without a valid key. */
		UnknownVersion,  /*!< The version of this data is unknown, or the data is otherwise not valid. */
		IntegrityFailed, /*!< The integrity check of the data failed. Perhaps the wrong key was used. */
	};

	/**
	  Constructor.

	  Constructs a SimpleCrypt instance without a valid key set on it.
	 */
	SimpleCryptQt();

	/**
	  Constructor.

	  Constructs a SimpleCrypt instance and initializes it with the given @arg key.
	 */
	explicit SimpleCryptQt(quint64 au8Key);

	/**
	  (Re-) initializes the key with the given @arg key.
	  */
	void mSetKey(quint64 au8Key);

	/**
	  Returns true if SimpleCrypt has been initialized with a key.
	  */
	bool hasKey() const {return !muvKeyParts.isEmpty();}

	/**
	  Sets the compression mode to use when encrypting data. The default mode is Auto.

	  Note that decryption is not influenced by this mode, as the decryption recognizes
	  what mode was used when encrypting.
	  */
	void mvSetCompressionMode(CompressionMode_e mode) {meCompressionMode = mode;}

	/**
	  Returns the CompressionMode that is currently in use.
	  */
	CompressionMode_e compressionMode() const {return meCompressionMode;}

	/**
	  Sets the integrity mode to use when encrypting data. The default mode is Checksum.

	  Note that decryption is not influenced by this mode, as the decryption recognizes
	  what mode was used when encrypting.
	  */
	void mvSetIntegrityProtectionMode(IntegrityProtectionMode_e mode) {meProtectionMode = mode;}

	/**
	  Returns the IntegrityProtectionMode that is currently in use.
	  */
	IntegrityProtectionMode_e integrityProtectionMode() const {return meProtectionMode;}

	/**
	  Returns the last error that occurred.
	  */
	Error_e lastError() const {return meLastError;}

	/**
	  Encrypts the @arg plaintext string with the key the class was initialized with, and returns
	  a cyphertext the result. The result is a base64 encoded version of the binary array that is the
	  actual result of the string, so it can be stored easily in a text format.
	  */
	QString msEncryptToString(const QString& asrPlaintext) ;

	/**
	  Encrypts the @arg plaintext QByteArray with the key the class was initialized with, and returns
	  a cyphertext the result. The result is a base64 encoded version of the binary array that is the
	  actual result of the encryption, so it can be stored easily in a text format.
	  */
	QString msEncryptToString(QByteArray asPlaintext) ;

	/**
	  Encrypts the @arg plaintext string with the key the class was initialized with, and returns
	  a binary cyphertext in a QByteArray the result.

	  This method returns a byte array, that is useable for storing a binary format. If you need
	  a string you can store in a text file, use encryptToString() instead.
	  */
	QByteArray mEncryptToByteArray(const QString& asrPlaintext) ;

	/**
	  Encrypts the @arg plaintext QByteArray with the key the class was initialized with, and returns
	  a binary cyphertext in a QByteArray the result.

	  This method returns a byte array, that is useable for storing a binary format. If you need
	  a string you can store in a text file, use encryptToString() instead.
	  */
	QByteArray mEncryptToByteArray(QByteArray asPlaintext) ;

	/**
	  Decrypts a cyphertext string encrypted with this class with the set key back to the
	  plain text version.

	  If an error occured, such as non-matching keys between encryption and decryption,
	  an empty string or a string containing nonsense may be returned.
	  */
	QString msDecryptToString(const QString& asrCyphertext) ;

	/**
	  Decrypts a cyphertext string encrypted with this class with the set key back to the
	  plain text version.

	  If an error occured, such as non-matching keys between encryption and decryption,
	  an empty string or a string containing nonsense may be returned.
	  */
	QByteArray msDecryptToByteArray(const QString& msrCypherText) ;

	/**
	  Decrypts a cyphertext binary encrypted with this class with the set key back to the
	  plain text version.

	  If an error occured, such as non-matching keys between encryption and decryption,
	  an empty string or a string containing nonsense may be returned.
	  */
	QString msDecryptToString(QByteArray asCypher) ;

	/**
	  Decrypts a cyphertext binary encrypted with this class with the set key back to the
	  plain text version.

	  If an error occured, such as non-matching keys between encryption and decryption,
	  an empty string or a string containing nonsense may be returned.
	  */
	QByteArray msDecryptToByteArray(QByteArray asCypher) ;

	//enum to describe options that have been used for the encryption. Currently only one, but
	//that only leaves room for future extensions like adding a cryptographic hash...
	enum class CryptoFlag_e {
		None        = 0,
		Compression = 0x01,
		Checksum    = 0x02,
		Hash        = 0x04
	};
	Q_DECLARE_FLAGS(CryptoFlags, CryptoFlag_e)

private:
	void mSplitKey();

	quint64 mu8Key;
	QVector<char> muvKeyParts;
	CompressionMode_e meCompressionMode;
	IntegrityProtectionMode_e meProtectionMode;
	Error_e meLastError;
};
Q_DECLARE_OPERATORS_FOR_FLAGS(SimpleCryptQt::CryptoFlags)

#endif // SIMPLECRYPT_H
