#include <iostream>
#include <vector>
#include <sstream>
#include <fstream>

class Tax {
    protected:
        double income;
    public:
        Tax(double inc) : income(inc) {}
        virtual ~Tax() = default;
        virtual double getTax() const = 0;
    };

class SimplifiedTax: public Tax
{
public:
    using Tax::Tax;
    double getTax() const override
    {
        double Res = (income / 100) * 5;
        if (income > 7820000)
        {
            Res += ((income - 7820000) / 100) * 15;
        }
        return Res;
    }
};

class GeneralTax : public Tax
{
public:
    using Tax::Tax;
    double getTax() const override
    {
        double Res = (income / 100) * 18;

        return Res;
    }
};

class SalaryTax : public Tax
{
public:
    using Tax::Tax;
    double getTax() const override
    {
        double Res = (income / 100) * 40;
        return Res;
    }
};

class TaxR
{
public:
    Tax* m_val;
    TaxR(Tax* val)
    {
        m_val = val;
    }

    ~TaxR()
    {
        delete m_val;
    }

    TaxR(const TaxR& obj) = delete;
    TaxR& operator=(const TaxR& obj) = delete;
    TaxR(TaxR&& obj)
    {
        m_val = obj.m_val;
        obj.m_val = nullptr;
    }
    TaxR& operator=(TaxR&& obj)
    {
        m_val = obj.m_val;
        obj.m_val = nullptr;
        return *this;
    }
};

Tax* createNewTax(const int id, double salary)
{
    if (id == 1)
        return new SimplifiedTax(salary);
    if (id == 2)
        return new GeneralTax(salary);
    if (id == 3)
        return new SalaryTax(salary);
    return nullptr;
}

int main()
{
    std::vector<TaxR> Vec;
    std::ifstream file("Income.csv");
    if (!file.is_open())
    {
        std::cerr << "Error" << std::endl;
        return 1;
    }

    std::string line;

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        std::string typeStr, valueStr;
        if (std::getline(ss, typeStr, ',') && std::getline(ss, valueStr))
        {
            int type = std::stoi(typeStr);
            double value = std::stod(valueStr);
            auto tax = createNewTax(type, value);
            if (tax)
            {
                Vec.push_back(tax);
            }
        }
    }
    for (const auto& tax : Vec) 
    {
        std::cout << "Tax: " << std::fixed << tax.m_val->getTax() << std::endl;
    }    
}