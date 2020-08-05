package il.co.ilrd.tomcat_server;

import java.io.UnsupportedEncodingException;
import java.util.Date;
import java.util.concurrent.TimeUnit;

import io.jsonwebtoken.Claims;
import io.jsonwebtoken.Jwts;
import io.jsonwebtoken.SignatureAlgorithm;

public class TokenManager {
	
	public static String generateToken(String email) {
		String token;
		try {
			token = Jwts.builder()
				//.setSubject("1234567890")
				//.setId("5c670a74-c8c9-4a21-a000-fd01b65c7773")
				.setIssuedAt(new Date())
				.setExpiration(new Date(System.currentTimeMillis() + TimeUnit.MINUTES.toMillis(60)))
				.claim("email", email)
				.signWith(SignatureAlgorithm.HS256, "ct,h kvmkhj".getBytes("UTF-8"))
				.compact();
		} catch (UnsupportedEncodingException e) {
			return null;
		}
		return token;
	}
	
	public static String getEmail(String token) {
		try {
			return getClaims(token).get("email").toString();			
		} catch (NullPointerException e) {
			return null;
		}
	}
	
	private static Claims getClaims(String token) {
        Claims claims;
        try {
            claims = Jwts.parser()
            	.setSigningKey("ct,h kvmkhj".getBytes("UTF-8"))
        		.parseClaimsJws(token)
                .getBody();
        } catch (Exception e) {
            return null;
        }
        return claims;
    }
}