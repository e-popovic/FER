package raytracing;

/**
 * <p>Title: Scene</p>
 * <p>Description: </p>
 * Klasa predstvlja scenu kod modela crtanja slike pomocu ray tracinga. Sastoji
 * se od izvora svjetlosti i konacnog broja objekata.
 * <p>Copyright: Copyright (c) 2003</p>
 * @author Milenka Gadze, Miran Mosmondor
 * @version 1.1
 */

import java.awt.*;
import javax.swing.*;
import java.awt.geom.*;
import java.awt.event.*;

public class Scene {

  final int MAXDEPTH=5; //maksimalna dubina rekurzije
  private int numberOfObjects;
  private Sphere[] sphere;
  private Point lightPosition;
  private ColorVector backroundColors=new ColorVector(0, 0, 0);
  private ColorVector light=new ColorVector((float)1 , (float)1,(float)1);
  private ColorVector ambientLight=new ColorVector((float)0.5, (float)0.5, (float)0.5);

  /**
   * Inicijalni konstruktor koji postavlja poziciju svijetla i parametre svih
   * objekata u sceni.
   *
   * @param lightPosition pozicija svijetla
   * @param numberOfObjects broj objekata u sceni
   * @param sphereParameters parametri svih kugli
   */
  public Scene(Point lightPosition, int numberOfObjects, SphereParameters[] sphereParameters) {
    this.lightPosition=lightPosition;
    this.numberOfObjects=numberOfObjects;
    sphere=new Sphere[numberOfObjects];
    for (int i=0; i<numberOfObjects; i++) {
      sphere[i]=new Sphere(sphereParameters[i]);
    }
  }

  /**
   * Metoda provjerava da li postoji sjena na tocki presjeka. Vraca true ako
   * se zraka od mjesta presjeka prema izvoru svjetlosti sjece s nekim objektom.
   *
   * @param intersection tocka presjeka
   * @return true ako postoji sjena u tocki presjeka, false ako ne postoji
   */
  private boolean shadow(Point intersection) {
	  //stvara se zraka izmedu svijetla i tocke
	  //racuna se postoji li presjek s bilokojom sferom, pomocu postojece metode intersection
	  Ray zraka=new Ray(intersection,lightPosition);
	  for (int i=0; i<numberOfObjects; i++) {
		  if (sphere[i].intersection(zraka))
			  return true;
	  }
	  return false;
  }

  /**
   * Metoda koja pomocu pracenja zrake racuna boju u tocki presjeka. Racuna se
   * osvjetljenje u tocki presjeka te se zbraja s doprinosima osvjetljenja koje
   * donosi reflektirana i refraktirana zraka.
   *
   * @param ray pracena zraka
   * @param depth dubina rekurzije
   * @return vektor boje u tocki presjeka
   */
  public ColorVector traceRay(Ray ray, int depth) {
	//implementacija pseudokoda iz 3. poglavlja upute 
    int najbliziIndeks = -1;
    boolean presjek=false;
    
    //ako je prosla maksimalna dubina koja nas zanima vraca se crna
	if (depth > MAXDEPTH) 
    	return new ColorVector(0,0,0);	
	
	//racuna se koji je najblizi presjek zrake, koja sfera se osvjetljuje (indeks sfere)
    for (int i=0; i<this.numberOfObjects; i++) {
    	if (sphere[i].intersection(ray)) {
    		presjek=true;
    		if (najbliziIndeks == -1) 
    			najbliziIndeks = i;
    		else {
    			double najblizi=ray.getStartingPoint().getDistanceFrom(sphere[najbliziIndeks].getIntersectionPoint());
        		double sad=ray.getStartingPoint().getDistanceFrom(sphere[i].getIntersectionPoint());
        		if (najblizi>sad)
        			najbliziIndeks=i;
    		}	
    	}
    }
    
    //ako nema presjeka sa sferom vraca se boja pozadine
    if (!presjek) {
    	return new ColorVector(backroundColors.getRed(),backroundColors.getGreen(),backroundColors.getBlue());
    }
    
    //ako postoji presjek sa sferom...
    else {
    	
    	//ambijentno osvjetljenje Ia * ka
    	//racuna se udio ambijentalnog osvjetljenja za svaku boju i mnozi se sa ambijentalnim osvjetljenjem
    	PropertyVector ambKoef=sphere[najbliziIndeks].ka;
    	float ra = ambKoef.getRedParameter()*ambientLight.getRed();
    	float ga = ambKoef.getGreenParameter()*ambientLight.getGreen();
    	float ba = ambKoef.getBlueParameter()*ambientLight.getBlue();
    	ColorVector Clocal = new ColorVector(ra,ga,ba);
    	
    	//difuzno osvjetljenje Ii * kd * (LxN)
    	PropertyVector difKoef=sphere[najbliziIndeks].kd;
    	Point intersectionPoint=sphere[najbliziIndeks].getIntersectionPoint();
    	Vector L=new Vector(intersectionPoint,lightPosition);			//vektor upadne zrake L
    	Vector N=sphere[najbliziIndeks].getNormal(intersectionPoint);	//vektor normale N
    	Vector V = new Vector(intersectionPoint,ray.getStartingPoint());//vektor smjera gledanja V
    	L.normalize();
    	N.normalize();
    	V.normalize();
    	
    	//ako je umnozak V i N negativan normala se treba okrenuti na suprotnu stranu (upute-3.8)
	  	double ni = sphere[najbliziIndeks].getNi();	
	  	double VN = V.dotProduct( N );
	  	if( VN < 0 ) {
	  	  N = N.multiple( -1 );  
			ni = 1./ni;
	  	}
    	
    	double LN= L.dotProduct(N);
    	
    	//spekularno osvjetljenje Ii * ks * (RxV)**n
    	PropertyVector speKoef=sphere[najbliziIndeks].ks;
    	Vector R = L.getReflectedVector(N);
	  	R.normalize();
    	double RV= R.dotProduct(V);
    	float n = sphere[najbliziIndeks].getN();
    	
    	
    	//racunanje i zbrajanje difuzne i spekularne komponente ambijentnoj
    	//potrebno ako nema sjene u toj tocki!!
    	if (!shadow(intersectionPoint)) {
    		if (LN>0) {
        		//difuzna
        		float rd = difKoef.getRedParameter()*light.getRed()*(float)LN;
            	float gd = difKoef.getGreenParameter()*light.getGreen()*(float)LN;
            	float bd = difKoef.getBlueParameter()*light.getBlue()*(float)LN;
            	ColorVector dif=new ColorVector(rd,gd,bd);
            	Clocal = Clocal.add(dif);
            	Clocal.correct();
        	}
    		if(RV>0) {	
        	//spekularna
        	float rs = speKoef.getRedParameter()*light.getRed()*(float)Math.pow(RV, n);
        	float gs = speKoef.getGreenParameter()*light.getGreen()*(float)Math.pow(RV, n);
        	float bs = speKoef.getBlueParameter()*light.getBlue()*(float)Math.pow(RV, n);
        	ColorVector spe=new ColorVector(rs,gs,bs);
        	Clocal = Clocal.add(spe);
        	Clocal.correct();
        	}
    	}
    	
    	//racunanje Rrefl
    	//Vrefl - vektor(izvor svijetlosti - presjek) reflektiran preko normale
    	//Rrefl - zraka s presjeka u smjeru Vrefl
    	Vector Vrefl=V.getReflectedVector(N);
    	Vrefl.normalize();
    	Ray Rrefl=new Ray(intersectionPoint,Vrefl);
    	
    	//racunanje reflektivne svjetlosti ~rekurzija~
    	ColorVector Crefl=traceRay(Rrefl, depth+1);
    	
    	//racunanje Rrefr
    	//Vrefr - vektor(izvor svijetlosti - presjek) refraktiran preko normale
    	//Rrefr - zraka s presjeka u smjeru Vrefr
    	Vector Vrefr=V.getRefractedVector(N, ni);
    	Vrefr.normalize();
    	Ray Rrefr=new Ray(intersectionPoint,Vrefr);
    	
    	//racunanje refraktirane svjetlosti ~rekurzija~
    	ColorVector Crefr=traceRay(Rrefr, depth+1);
    	
    	//I = Iilum + k1 * Irefl
    	ColorVector C=Clocal.add(Crefl.multiple(sphere[najbliziIndeks].getReflectionFactor()));
    	//I = Iilum + k1 * Irefl + k2 * Irefr
    	C=C.add(Crefr.multiple(sphere[najbliziIndeks].getRefractionFactor()));
    	
    	C.correct();
    	return C;	
    } 
  }
}