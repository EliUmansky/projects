package il.co.ilrd.tomcat_server;

import java.security.InvalidKeyException;
import java.security.Key;
import java.security.NoSuchAlgorithmException;

import javax.crypto.BadPaddingException;
import javax.crypto.Cipher;
import javax.crypto.IllegalBlockSizeException;
import javax.crypto.NoSuchPaddingException;
import javax.crypto.spec.SecretKeySpec;
import javax.xml.bind.DatatypeConverter;

import org.apache.tomcat.util.codec.binary.Base64;

public class Encriptor {

	public static String encript(String input) {
		if(input == null){
            return null;
        }

        String keyString = "C0BAE23DF8B51807B3E17D21925FADF2";//32 byte string
        byte[] keyValue = DatatypeConverter.parseHexBinary(keyString);
        Key key = new SecretKeySpec(keyValue, "AES");
        Cipher c1 = null;
        try {
            c1 = Cipher.getInstance("AES");
        } catch (NoSuchAlgorithmException | NoSuchPaddingException e) {
            return null;
        }
        try {
            c1.init(Cipher.ENCRYPT_MODE, key);
        } catch (InvalidKeyException e) {
            return null;
        }

        byte[] encVal = null;
        try {
            encVal = c1.doFinal(input.getBytes());
        } catch (IllegalBlockSizeException | BadPaddingException e) {
            return null;
        }
        String encryptedValue = Base64.encodeBase64String(encVal);

        return encryptedValue;
	}

	/*
	public static String encript(String password) {
        String key = "Bar12345Bar12345"; // 128 bit key
         // Create key and cipher
        Key aesKey = new SecretKeySpec(key.getBytes(), "AES");
        Cipher cipher;
        StringBuilder sb = null;
		
        try {
			cipher = Cipher.getInstance("AES");
			cipher.init(Cipher.ENCRYPT_MODE, aesKey);
			byte[] encrypted = cipher.doFinal(password.getBytes());
			sb = new StringBuilder();
			for (byte b: encrypted) {
				sb.append((char)b);
			}
		} catch (Exception e) {
			e.printStackTrace();
		}

        return sb.toString();
	}
	
	public static String decript(String encripted) {
		Cipher cipher;
		String key = "Bar12345Bar12345"; // 128 bit key
		Key aesKey = new SecretKeySpec(key.getBytes(), "AES");
		byte[] bb = new byte[encripted.length()];
		for (int i=0; i<encripted.length(); i++) {
			bb[i] = (byte) encripted.charAt(i);
		}
		String decrypted = null;
		try {
			cipher = Cipher.getInstance("AES");
			cipher.init(Cipher.DECRYPT_MODE, aesKey);
			decrypted = new String(cipher.doFinal(bb));
		} catch (Exception e) {
			e.printStackTrace();
		} 

		return decrypted;
	}
	*/
}
