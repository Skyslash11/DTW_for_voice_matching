import java.io.*;
import java.lang.*;
import java.util.Vector;
public class DTW {
  public static int MAX_VALUE=1000000000;
  public static void main(String[] args) {
    compare("ashitan.wav","ashn.wav");
  }
  public static void compare(String st1,String st2)
  {
    int bytes, cursor, unsigned;
    Vector<Vector<Integer> > vct1 = new Vector<Vector<Integer> >();
    Vector<Vector<Integer> > vct2 = new Vector<Vector<Integer> >();
    int count1=0;
    int count2=0;
    
    try {
      FileInputStream s = new FileInputStream(st1);
      BufferedInputStream b = new BufferedInputStream(s);
      byte[] data = new byte[128];
      b.skip(44);
      cursor = 0;
      
      //int count=0;
      while ((bytes = b.read(data)) > 0) {
        // do something
        Vector<Integer> tempVct1 = new Vector<Integer>();
        vct1.add(tempVct1);
        for(int i=0; i<bytes; i++) {
                (vct1.get(count1)).add(data[i] & 0xFF); // Java..
                //System.out.println(cursor + " " + (vct1.get(count)).get(i));
                cursor++;
        }
        ++count1;
      }
      b.read(data);
      b.close();
    } catch(Exception e) {
      e.printStackTrace();
    }
	
    try {
      FileInputStream s = new FileInputStream(st2);
      BufferedInputStream b = new BufferedInputStream(s);
      byte[] data = new byte[128];
      b.skip(44);
      cursor = 0;
      //int count=0;
      while ((bytes = b.read(data)) > 0) {
        // do something
        Vector<Integer> tempVct2 = new Vector<Integer>();
        vct2.add(tempVct2);
        for(int i=0; i<bytes; i++) {
                (vct2.get(count2)).add(data[i] & 0xFF); // Java..
                //System.out.println(cursor + " " + (vct2.get(count)).get(i));
                cursor++;
        }
        ++count2;
      }
      b.read(data);
      b.close();
    } catch(Exception e) {
      e.printStackTrace();
    }
    count1=count1/10;
    count2=count2/10;
    //System.out.println(count2);
    int[][] dp=new int[count1][count2];
    for(int i=0;i<count1;++i)
	{
		for(int j=0;j<count2;++j)
		{
			if(i==0 && j==0)
				dp[i][j]=calculate_distance(vct1.get(i),vct2.get(j));
			else if(i==0 && j!=0)
			{
				dp[i][j]=calculate_distance(vct1.get(i),vct2.get(j))+dp[i][j-1];
				if(dp[i][j]/MAX_VALUE >= 1)
					dp[i][j]=MAX_VALUE;
			}
			else if(i!=0 && j==0)
			{
				dp[i][j]=calculate_distance(vct1.get(i),vct2.get(j))+dp[i-1][j];
				if(dp[i][j]/MAX_VALUE >= 1)
					dp[i][j]=MAX_VALUE;
			}
			else
			{
				//printf("_______%d\n",vct1.get(i)[0]-vct2.get(j)[0]);
				
				dp[i][j]=calculate_distance(vct1.get(i),vct2.get(j))+find_min(dp[i][j-1],dp[i-1][j-1],dp[i-1][j]);
				if(dp[i][j]/MAX_VALUE >= 1)
					dp[i][j]=MAX_VALUE;
			}
		}
	}
  //System.out.println(dp[count1-1][count2-1]);
  }
  public static int calculate_distance(Vector<Integer> sp1,Vector<Integer> sp2)
	{
		double distance=0;
		int a=0,b=0;
                //System.out.println(sp2.size());
                if(sp1.size()!=128 || sp2.size()!=128)
                    return MAX_VALUE;
		for(int i=0;i<128;++i)
		{
			distance=distance+Math.sqrt(((sp1.get(i)-sp2.get(i))/100)*((sp1.get(i)-sp2.get(i))/100));
			//a+=sp1[i]/512;
			//b+=sp2[i]/512;
		}	
		//printf("_______%d\n",sp1[0]-sp2[0]);
		return (int)distance;
	}

   public static int find_min(int a,int b,int c)
	{
		int min=MAX_VALUE;
		if(a<b && a>=0 && b>=0)
			min=a;
		else if (a>=b && a>=0 && b>=0)
			min=b;
		else
			min=MAX_VALUE;
		if(c<min && c>=0)
			min=c;
		return min;
	}
}
