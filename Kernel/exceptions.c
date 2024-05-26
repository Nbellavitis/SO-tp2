
#define ZERO_EXCEPTION_ID 0
#define INVALID_OPCODE_ID 6

static void zero_division();
static void invalid_opcode();

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID){
		zero_division();
	}else{
		invalid_opcode();
		}
	//Se deben imprimir los registros
}

static void zero_division() {
	// Handler para manejar excepcíon
	
}

static void invalid_opcode(){

	//Deberia imprir error fatal excepcion ...

}