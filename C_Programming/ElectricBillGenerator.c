//=================================================================//
//                ELECTRICITY BILL GENERATOR                       //
//=================================================================//

#include <stdio.h>
#include <string.h>
#include <time.h>

/* ===================== CONSTANTS ===================== */
#define FIXED_CHARGES 40.00f
#define METER_RENT 40.00f
#define ELECTRICITY_DUTY 8.24f

#define PROMPT_REBATE -6.30f
#define RURAL_REBATE -6.30f
#define DIGITAL_REBATE -9.34f

#define LOWER_UNIT_RATE 2.90f
#define HIGHER_UNIT_RATE 4.70f

#define SLAB_LIMIT 50
#define DAYS_PER_MONTH 30
#define LATE_FEE 25.00f

/* ===================== STRUCTURES ===================== */
typedef struct
{
  char at[50];
  char post[30];
  char district[30];
} Address;

typedef struct
{
  char billNumber[21];
  char date[30];
  char division[30];
  char subDivision[30];
  char sector[30];
} BillDetails;

typedef struct
{
  char scNumber[13];
  char acNumber[12];
  char oldAcNumber[8];
  char mobile[11];
  char email[50];
  char name[50];
  Address address;
} Customer;

typedef struct
{
  char meterNumber[15];
  char ownerName[50];
  char prevReadingDate[15];
  char currReadingDate[15];

  int units;
  int months;
  int days;

  float energyCharge;
  float totalAmount;

  char rebateDate[15];
  char discountDate[15];
} MeterInfo;

/* ===================== UTILITY FUNCTIONS ===================== */

void removeNewLine(char *str)
{
  str[strcspn(str, "\n")] = '\0';
}

void getCurrentDateTime(char *buffer, size_t size)
{
  time_t now = time(NULL);
  struct tm *t = localtime(&now);
  strftime(buffer, size, "%Y-%m-%d %H:%M:%S", t);
}

void clearInputBuffer()
{
  int c;
  while ((c = getchar()) != '\n' && c != EOF)
    ;
}

/* ===================== INPUT FUNCTIONS ===================== */

void inputBillDetails(BillDetails *bill)
{
  printf("Enter Bill Number: ");
  fgets(bill->billNumber, sizeof(bill->billNumber), stdin);
  removeNewLine(bill->billNumber);

  getCurrentDateTime(bill->date, sizeof(bill->date));

  printf("Enter Division: ");
  fgets(bill->division, sizeof(bill->division), stdin);
  removeNewLine(bill->division);

  printf("Enter Sub-Division: ");
  fgets(bill->subDivision, sizeof(bill->subDivision), stdin);
  removeNewLine(bill->subDivision);

  printf("Enter Sector: ");
  fgets(bill->sector, sizeof(bill->sector), stdin);
  removeNewLine(bill->sector);
}

void inputCustomerDetails(Customer *cust)
{
  printf("Enter SC Number: ");
  fgets(cust->scNumber, sizeof(cust->scNumber), stdin);
  removeNewLine(cust->scNumber);

  clearInputBuffer();

  printf("Enter AC Number: ");
  fgets(cust->acNumber, sizeof(cust->acNumber), stdin);
  removeNewLine(cust->acNumber);

  printf("Enter Old AC Number: ");
  fgets(cust->oldAcNumber, sizeof(cust->oldAcNumber), stdin);
  removeNewLine(cust->oldAcNumber);

  printf("Enter Mobile Number: ");
  fgets(cust->mobile, sizeof(cust->mobile), stdin);
  removeNewLine(cust->mobile);

  clearInputBuffer();

  printf("Enter Email: ");
  fgets(cust->email, sizeof(cust->email), stdin);
  removeNewLine(cust->email);

  printf("Enter Customer Name: ");
  fgets(cust->name, sizeof(cust->name), stdin);
  removeNewLine(cust->name);

  printf("Enter Address (At): ");
  fgets(cust->address.at, sizeof(cust->address.at), stdin);
  removeNewLine(cust->address.at);

  printf("Enter Post: ");
  fgets(cust->address.post, sizeof(cust->address.post), stdin);
  removeNewLine(cust->address.post);

  printf("Enter District: ");
  fgets(cust->address.district, sizeof(cust->address.district), stdin);
  removeNewLine(cust->address.district);
}

void inputMeterInfo(MeterInfo *meter)
{
  printf("Enter Meter Number: ");
  fgets(meter->meterNumber, sizeof(meter->meterNumber), stdin);
  removeNewLine(meter->meterNumber);

  printf("Enter Meter Owner Name: ");
  fgets(meter->ownerName, sizeof(meter->ownerName), stdin);
  removeNewLine(meter->ownerName);

  printf("Enter Previous Reading Date: ");
  fgets(meter->prevReadingDate, sizeof(meter->prevReadingDate), stdin);
  removeNewLine(meter->prevReadingDate);

  printf("Enter Current Reading Date: ");
  fgets(meter->currReadingDate, sizeof(meter->currReadingDate), stdin);
  removeNewLine(meter->currReadingDate);

  printf("Enter Units Consumed: ");
  scanf("%d", &meter->units);

  printf("Enter Billing Months: ");
  scanf("%d", &meter->months);

  clearInputBuffer();

  meter->days = meter->months * DAYS_PER_MONTH;

  printf("Enter Rebate Date: ");
  fgets(meter->rebateDate, sizeof(meter->rebateDate), stdin);
  removeNewLine(meter->rebateDate);

  printf("Enter Discount Date: ");
  fgets(meter->discountDate, sizeof(meter->discountDate), stdin);
  removeNewLine(meter->discountDate);
}

/* ===================== BILL CALCULATION ===================== */

float calculateEnergyCharge(int units)
{
  if (units <= SLAB_LIMIT)
    return units * LOWER_UNIT_RATE;

  return (SLAB_LIMIT * LOWER_UNIT_RATE) +
         ((units - SLAB_LIMIT) * HIGHER_UNIT_RATE);
}

float calculateTotalAmount(MeterInfo *meter)
{
  float amount = meter->energyCharge +
                 FIXED_CHARGES +
                 METER_RENT +
                 ELECTRICITY_DUTY;

  amount += PROMPT_REBATE + RURAL_REBATE + DIGITAL_REBATE;
  return amount;
}

/* ===================== DISPLAY ===================== */

void displayBill(const Customer *cust,
                 const BillDetails *bill,
                 MeterInfo *meter)
{
  meter->energyCharge = calculateEnergyCharge(meter->units);
  meter->totalAmount = calculateTotalAmount(meter);

  printf("\n================ ELECTRICITY BILL =================\n");
  printf("TPSODL - Tata Power Southern Odisha Distribution Ltd\n\n");

  printf("Bill No          : %s\n", bill->billNumber);
  printf("Bill Date        : %s\n", bill->date);
  printf("Division         : %s\n", bill->division);
  printf("Sub-Div          : %s\n", bill->subDivision);
  printf("Sector           : %s\n", bill->sector);

  printf("---------------------------------------------------\n");

  printf("SC No            : %s\n", cust->scNumber);
  printf("AC No            : %s\n", cust->acNumber);
  printf("Old AC No        : %s\n", cust->oldAcNumber);
  printf("Customer         : %s\n", cust->name);
  printf("Mobile           : %s\n", cust->mobile);
  printf("Email            : %s\n", cust->email);
  printf("Address          : At-%s ,Po.-%s ,Dist.-%s\n",
         cust->address.at,
         cust->address.post,
         cust->address.district);

  printf("---------------------------------------------------\n");

  printf("Meter No         : %s\n", meter->meterNumber);
  printf("Meter Owner      : %s\n", meter->ownerName);
  printf("Units Consumed   : %d\n", meter->units);
  printf("Billing Months   : %d\n", meter->months);
  printf("Billing Days     : %d\n", meter->days);

  printf("---------------------------------------------------\n");

  printf("\t\tBill Slab Details\n");
  if (meter->units <= SLAB_LIMIT)
  {
    printf("%d * %.2f = %.2f\n", meter->units, LOWER_UNIT_RATE,
           meter->units * LOWER_UNIT_RATE);
  }
  else
  {
    printf("%d * %.2f = %.2f\n", SLAB_LIMIT, LOWER_UNIT_RATE,
           SLAB_LIMIT * LOWER_UNIT_RATE);
    printf("%d * %.2f = %.2f\n", meter->units - SLAB_LIMIT, HIGHER_UNIT_RATE,
           (meter->units - SLAB_LIMIT) * HIGHER_UNIT_RATE);
  }

  printf("---------------------------------------------------\n");

  printf("Energy Charge    : %.2f\n", meter->energyCharge);
  printf("Fixed Charges    : %.2f\n", FIXED_CHARGES);
  printf("Meter Rent       : %.2f\n", METER_RENT);
  printf("Electricity Duty : %.2f\n", ELECTRICITY_DUTY);
  printf("Curr. AMT        : %.2f\n",
         meter->energyCharge + FIXED_CHARGES + METER_RENT + ELECTRICITY_DUTY);
  printf("Prompt REBT      : %.2f\n", PROMPT_REBATE);
  printf("Rural REBT       : %.2f\n", RURAL_REBATE);
  printf("BEF. REBT DT     : %.2f\n", meter->energyCharge + FIXED_CHARGES + METER_RENT + ELECTRICITY_DUTY + PROMPT_REBATE + RURAL_REBATE);
  printf("Digital REBT     : %.2f\n", DIGITAL_REBATE);
  printf("ONLINE CURR.BL \n");
  printf("BY DUE DT        : %.2f\n", meter->totalAmount);
  printf("AFT. REBT DT     : %.2f\n",
         meter->totalAmount + LATE_FEE);
  printf("REBT DT         : %s\n", meter->rebateDate);
  printf("DISC DT          : %s\n", meter->discountDate);

  printf("===================================================\n");
}

/* ===================== MAIN ===================== */

int main()
{
  BillDetails bill;
  Customer customer;
  MeterInfo meter;

  inputBillDetails(&bill);
  inputCustomerDetails(&customer);
  inputMeterInfo(&meter);

  displayBill(&customer, &bill, &meter);

  return 0;
}

// OUTPUT ARE HERE BELOW
/*================ ELECTRICITY BILL ================
TPSODL - Tata Power Southern Odisha Distribution Ltd

Bill No          : BIL202601060001
Bill Date        : 2026-01-06 18:25:58
Division         : North Division
Sub-Div          : Sub-Division A
Sector           : Sector 5
--------------------------------------------------
SC No            : SC123456
AC No            : AC654321
Old AC No        : AC78901
Customer         : john Doe
Mobile           : 6589432395
Email            : johndoe@example.com
Address          : At-kuntia ,Po.-durgapur ,Dist.-porbandar
---------------------------------------------------
Meter No         : MTR987654321
Meter Owner      : TPSOLD
Units Consumed   : 63
Billing Months   : 1
Billing Days     : 30
--------------------------------------------------
                Bill Slab Details
50 * 2.90 = 145.00
13 * 4.70 = 61.10
--------------------------------------------------
Energy Charge    : 206.10
Fixed Charges    : 40.00
Meter Rent       : 40.00
Electricity Duty : 8.24
Curr. AMT        : 294.34
Prompt REBT      : -6.30
Rural REBT       : -6.30
BEF. REBT DT     : 282.00
Digital REBT     : -9.34
ONLINE CURR.BL
BY DUE DT        : 272.40
AFT. REBT DT     : 297.40
REBT DT         : 2026-01-20
DISC DT          : 2026-01-25
===================================================*/