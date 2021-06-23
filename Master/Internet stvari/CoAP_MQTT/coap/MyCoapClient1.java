package coap;

import org.eclipse.californium.core.CoapClient;
import org.eclipse.californium.core.CoapResponse;

public class MyCoapClient1 {

	public static void main(String[] args) {
		
		CoapClient client = new CoapClient("coap://127.0.0.1:5683/sensors/temp");
		
		try {
			//CON (1a)
			CoapResponse contentCon = client.useCONs().get(); 
			System.out.println( "CON" );
			if (contentCon!=null) {		        
	        	System.out.println( contentCon.getCode() );
	        	System.out.println( contentCon.getOptions() );
	        	System.out.println( contentCon.getResponseText() );	        	
	        } else {	        	
	        	System.out.println("Con request failed\n");	        	
	        }
			
			//NON (1b)
			CoapResponse contentNon = client.useNONs().get(); 
			System.out.println( "NON" );
			if (contentNon!=null) {		        
	        	System.out.println( contentNon.getCode() );
	        	System.out.println( contentNon.getOptions() );
	        	System.out.println( contentNon.getResponseText() );	        	
	        } else {	        	
	        	System.out.println("Non request failed\n");	        	
	        }
		} 
		catch (Exception e) { }
	}
	
}
