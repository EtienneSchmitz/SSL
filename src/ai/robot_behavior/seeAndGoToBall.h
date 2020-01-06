namespace rhoban_ssl
{
    namespace robot_behavior
    {
        class seeAndGoToBall
        {
        private:
            /* data */
        public:
            virtual void seeAndGoToBall(/* args */);

            virtual Control control() const;

            virtual rhoban_ssl::annotations::Annotations getAnnotations() const;

            void seeBall();

            void go_to_ball();

            virtual ~seeAndGoToBall();
        };
        
        seeAndGoToBall::seeAndGoToBall(/* args */)
        {
        }
        
        seeAndGoToBall::~seeAndGoToBall()
        {
        }
        
    }
}