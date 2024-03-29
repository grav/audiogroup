import java.util.Arrays; 
import java.util.Random;

import junit.framework.TestCase;


public class TestACCoder extends TestCase {

	public ACCoder lumCoder;
	
	protected void setUp() throws Exception {
		super.setUp();
		lumCoder = new ACCoder(ACCoder.Channel.LUMINANCE);
	}

	public void testEncode(){		
		// 1 nul (1,4)
		assertEquals("11111011010001010",lumCoder.encode(new int[]{0,8}));

		// 15 nuller (15,0) samt (0,4)
		assertEquals("11111111001101110001010",lumCoder.encode(new int[]{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8}));
		
		// 16 nuller (15,0) samt (1,4)
		assertEquals("1111111100111111011010001010",lumCoder.encode(new int[]{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,8}));
	
		//           |-0/6--|--48-|0/4|-12|0/4|-10|0/4|--8|EOB|
		assertEquals("11110001100001011110010111010101110001010",
				lumCoder.encode(new int[]{48,12,10,8}));
				
	}
	
	
	
	
	public void testZigzag(){
		int[][] block = {
				{ 48, 12,  0,  6},
				{-10,  8,  0,  4},
				{  2,  0,  7, 14},
				{  4,  0,  0, -6}
		};
		assertTrue(Arrays.equals(
						new int[] {48,12,-10,2,8,0,6,0,0,4,0,7,4,14,0,-6}, 
						lumCoder.zigzag(block)));

		int[][] block2 = {
				{ 48, 12,  0,  6,  5},
				{-10,  8,  0,  4, 13},
				{  2,  0,  7, 14, -3},
				{  4,  0,  0, -6, 11},
				{ 15,  3,  1,-16,-33},
		};
		assertTrue(Arrays.equals(
						new int[] {48,12,-10,2,8,0,6,0,0,4,15,0,7,4,5,13,14,0,3,1,-6,-3,11,-16,-33}, 
						lumCoder.zigzag(block2)));
		
		int[][] block3 = {
				{1,2,3,4,5,6,7,8},
				{9,10,11,12,13,14,15,16},
				{17,18,19,20,21,22,23,24},
				{25,26,27,28,29,30,31,32},
				{33,34,35,36,37,38,39,40},
				{41,42,43,44,45,46,47,48},
				{49,50,51,52,53,54,55,56},
				{57,58,59,60,61,62,63,64}
		};
		
		// Test against hardcoded example
		assertTrue(Arrays.equals(
				new int[] {1, 2, 9, 17, 10, 3, 4, 11, 18, 25, 33, 26, 19, 12, 5, 6, 13, 20, 27, 34, 41, 49, 42, 35, 28, 21, 14, 7, 8, 15, 22, 29, 36, 43, 50, 57, 58, 51, 44, 37, 30, 23, 16, 24, 31, 38, 45, 52, 59, 60, 53, 46, 39, 32, 40, 47, 54, 61, 62, 55, 48, 56, 63, 64},
				lumCoder.zigzag(block3)));
		
	}
	
	public void testAntiZigzag(){
		int[] c = {48,12,-10,2,8,0,6,0,0,4,15,0,7,4,5,13,14,0,3,1,-6,-3,11,-16,-33};
		int[][] block = lumCoder.antiZigzag(c, 5, 5);
		assertTrue(Arrays.equals(new int[] { 48, 12,  0,  6,  5}, block[0]));
		assertTrue(Arrays.equals(new int[] {-10,  8,  0,  4, 13}, block[1]));
		assertTrue(Arrays.equals(new int[] {  2,  0,  7, 14, -3}, block[2]));
		assertTrue(Arrays.equals(new int[] {  4,  0,  0, -6, 11}, block[3]));
		assertTrue(Arrays.equals(new int[] { 15,  3,  1,-16,-33}, block[4]));
	}
	
	public void testDecode(){
		int[] c;
		
		// decode one single EOB
		c = lumCoder.decode(
				"1010",
				64); 
		for(int i=0;i<64;i++){
			assertEquals(c[i],0);
		}

		// decode 2x2 matrix
		c = lumCoder.decode("11110001100001011110010111010101110001010",4);
		assertTrue(Arrays.equals(c,new int[] {48,12,10,8}));
		
		// decode 5x5 matrix
		c = lumCoder.decode(
				"111100011000010111100101110100110101110001111001110111111011110010111111111100111110010010010110111101101111101101111001100110011110111011110101000011110001000011010",
				25);
		assertTrue(Arrays.equals(c, new int[] {48,12,10,2,8,0,6,0,0,4,15,0,7,4,5,13,14,0,3,1,6,3,11,16,33}));
	
	}
	
	// Test en- and decoding of random block
	public void testEncodeDecode(){
		Random r = new Random();
		int s = 64;
		int[] c = new int[s];
		
		for(int i=0;i<s;i++){
			c[i]=r.nextInt(256);
		}
		
		assertTrue(
				Arrays.equals(c, 
						lumCoder.decode(
								lumCoder.encode(c), s)
				)
		);
		
	}
	
	public void testJava(){
		assertTrue(new Integer(5).equals(5));
		assertTrue(new Pair<Integer,Integer>(0,1).equals(new Pair<Integer,Integer>(0,1)));

	}
	
}
