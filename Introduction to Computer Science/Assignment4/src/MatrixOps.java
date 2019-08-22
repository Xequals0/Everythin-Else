public class MatrixOps
{
	public static double[][] multiply(double[][] matrix1, double[][] matrix2)
	{
		// rows System.out.println(matrix1.length); 
		//Columns System.out.println(a); 
		double [][] product = new double [matrix1.length][matrix2[0].length];
		double sumProd = 0;
		
		int columnsInMatrix1 = matrix1[0].length;
		int rowsInMatrix2 = matrix2.length;
		if(columnsInMatrix1 != rowsInMatrix2)
		{
			return null;
		}
		
		for(int i = 0; i < product.length;i++){
			for(int j = 0; j < product[0].length;j++){
				
				for(int x = 0; x < matrix2.length;x++){  // note: matrix2.length == matrix1[0].length
					sumProd += matrix1[i][x] * matrix2[x][j];  
				}
				product [i][j] = sumProd;
				sumProd = 0;
			}
		}
		return product;
	}
}