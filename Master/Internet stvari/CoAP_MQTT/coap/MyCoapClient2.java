package coap;

import org.eclipse.californium.core.CoapClient;
import org.eclipse.californium.core.CoapResponse;

public class MyCoapClient2 {

	public static void main(String[] args) {
		
		CoapClient client = new CoapClient("coap://127.0.0.1:5683/sensors");
		
		try {
			CoapResponse contentPost = client.useCONs().post("temp", 0);	// stvori temp resurs
			
			CoapResponse contentGet = client.useCONs().get();	// probaj GET (method not allowed!)
			
			if (contentPost!=null) {		        
	        	System.out.println( contentPost.getCode() );
	        	System.out.println( contentPost.getOptions() );
	        	System.out.println( contentPost.getResponseText() );	        	
	        } else {	        	
	        	System.out.println("Failed\n");	        	
	        }
			
			if (contentGet!=null) {		        
	        	System.out.println( contentGet.getCode() );
	        	System.out.println( contentGet.getOptions() );
	        	System.out.println( contentGet.getResponseText() );	        	
	        } else {	        	
	        	System.out.println("Failed\n");	        	
	        }

		} 
		catch (Exception e) { }      
	}
}
